__author__ = 'marco'

import wx
import AnimaEngine

try:
    from wx import glcanvas
    haveGlCanvas = True
except ImportError:
    haveGlCanvas = False

try:
    from OpenGL.GL import *
    from OpenGL.GLU import *
    haveOpenGl = True
except ImportError:
    haveOpenGl = False

class AnimaEditorCanvas(glcanvas.GLCanvas):
    def __init__(self, parent):
        glcanvas.GLCanvas.__init__(self, parent, -1)

        self.init = False
        self.initRenderer = False
        self.context = glcanvas.GLContext(self)

        self.engine = AnimaEngine.AnimaEngine()
        self.engine.Initialize()

        scenesManager = self.engine.GetScenesManager()
        assert isinstance(scenesManager, AnimaEngine.AnimaScenesManager)

        self.scene = scenesManager.CreateScene("test-scene")
        self.renderer = AnimaEngine.AnimaRenderer(self.engine, None)

        self.size = None
        self.Bind(wx.EVT_ERASE_BACKGROUND, self.OnEraseBackground)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_PAINT, self.OnPaint)

    def OnEraseBackground(self):
        pass

    def OnSize(self, event):
        wx.CallAfter(self.SetViewport)
        event.Skip()

    def OnPaint(self, event):
        dc = wx.PaintDC(self)

        self.SetCurrent(self.context)

        if not self.init:
            self.InitGl()
        self.OnDraw()
        self.SwapBuffers()

    def InitGl(self):
        result = AnimaEngine.AnimaGC_InitializeGLEWExtensions()
        if result == True:
            assert isinstance(self.scene, AnimaEngine.AnimaScene)
            shadersManager = self.scene.GetShadersManager()
            assert isinstance(shadersManager, AnimaEngine.AnimaShadersManager)

            prepareProgram = shadersManager.CreateProgram("deferred-prepare")
            assert isinstance(prepareProgram, AnimaEngine.AnimaShaderProgram)
            prepareProgram.Create()
            prepareProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-prepare-vs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Deferred/deferred-prepare-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            prepareProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-prepare-fs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Deferred/deferred-prepare-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            prepareProgram.Link()

            shadowProgram = shadersManager.CreateProgram("deferred-shadowMap")
            assert isinstance(shadowProgram, AnimaEngine.AnimaShaderProgram)
            shadowProgram.Create()
            shadowProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-shadowMap-vs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Deferred/deferred-shadowMap-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            shadowProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-shadowMap-fs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Deferred/deferred-shadowMap-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            shadowProgram.Link()

            combineProgram = shadersManager.CreateProgram("deferred-combine")
            assert isinstance(combineProgram, AnimaEngine.AnimaShaderProgram)
            combineProgram.Create()
            combineProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-combine-vs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Deferred/deferred-combine-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            combineProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-combine-fs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Deferred/deferred-combine-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            combineProgram.Link()

            primitiveDrawProgram = shadersManager.CreateProgram("primitive-draw")
            assert isinstance(primitiveDrawProgram, AnimaEngine.AnimaShaderProgram)
            primitiveDrawProgram.Create()
            primitiveDrawProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-draw-vs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Primitive/primitive-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            primitiveDrawProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-draw-fs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Primitive/primitive-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            primitiveDrawProgram.Link()

            primitiveCombineProgram = shadersManager.CreateProgram("primitive-combine")
            assert isinstance(primitiveCombineProgram, AnimaEngine.AnimaShaderProgram)
            primitiveCombineProgram.Create()
            primitiveCombineProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-combine-vs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Primitive/combine-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            primitiveCombineProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-combine-fs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Primitive/combine-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            primitiveCombineProgram.Link()

            fxaaFilterProgram = shadersManager.CreateProgram("fxaaFilter")
            assert isinstance(fxaaFilterProgram, AnimaEngine.AnimaShaderProgram)
            fxaaFilterProgram.Create()
            fxaaFilterProgram.AddShader(shadersManager.LoadShaderFromFile("fxaaFilter-vs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Filters/fxaaFilter-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            fxaaFilterProgram.AddShader(shadersManager.LoadShaderFromFile("fxaaFilter-fs", "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Filter/fxaaFilter-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            fxaaFilterProgram.Link()

            self.init = True

    def SetViewport(self):
        size = self.GetClientSize()

        self.renderer.InitRenderingTargets(size.width, size.height)
        self.renderer.InitRenderingUtilities(size.width, size.height)

        self.initRenderer = True

    def OnDraw(self):
        if self.init == True and self.initRenderer == True:
            self.renderer.Start(self.scene)
            self.renderer.DrawAll()

