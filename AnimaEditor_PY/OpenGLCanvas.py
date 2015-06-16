__author__ = 'Marco'

import wx
import AnimaEngine

class OpenGLCanvas(wx.Window):
    def __init__(self, parent):
        wx.Window.__init__(self, parent)

        self.init = False
        self.initRenderer = False

        windowId = self.GetHandle()
        fbConfig = AnimaEngine.AnimaGC_GetDefaultFrameBufferConfig()
        ctConfig = AnimaEngine.AnimaGC_GetDefaultContextConfig()

        self.context = AnimaEngine.AnimaGC_CreateContext(windowId, ctConfig, fbConfig)
        assert isinstance(self.context, AnimaEngine.AnimaGC)
        self.context.MakeCurrent()
        result = AnimaEngine.AnimaGC_InitializeGLEWExtensions()

        self.engine = AnimaEngine.AnimaEngine()
        self.engine.Initialize()
        scenesManager = self.engine.GetScenesManager()
        assert isinstance(scenesManager, AnimaEngine.AnimaScenesManager)
        self.scene = scenesManager.CreateScene("test-scene")
        self.renderer = AnimaEngine.AnimaRenderer(self.engine, None)

        self.Bind(wx.EVT_ERASE_BACKGROUND, self.OnEraseBackground)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_PAINT, self.OnPaint)

    def OnEraseBackground(self, event):
        pass

    def OnSize(self, event):
        wx.CallAfter(self.SetViewport)
        event.Skip()

    def OnPaint(self, event):
        dc = wx.PaintDC(self)

        self.context.MakeCurrent()

        if not self.init:
            self.InitGl()
        self.OnDraw()
        self.context.SwapBuffers()

    def InitGl(self):
        result = AnimaEngine.AnimaGC_InitializeGLEWExtensions()
        if result == True:
            assert isinstance(self.scene, AnimaEngine.AnimaScene)
            shadersManager = self.scene.GetShadersManager()
            assert isinstance(shadersManager, AnimaEngine.AnimaShadersManager)

            prepareProgram = shadersManager.CreateProgram("deferred-prepare")
            assert isinstance(prepareProgram, AnimaEngine.AnimaShaderProgram)
            prepareProgram.Create()
            prepareProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-prepare-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-prepare-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            prepareProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-prepare-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-prepare-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            prepareProgram.Link()

            shadowProgram = shadersManager.CreateProgram("deferred-shadowMap")
            assert isinstance(shadowProgram, AnimaEngine.AnimaShaderProgram)
            shadowProgram.Create()
            shadowProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-shadowMap-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-shadowMap-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            shadowProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-shadowMap-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-shadowMap-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            shadowProgram.Link()

            combineProgram = shadersManager.CreateProgram("deferred-combine")
            assert isinstance(combineProgram, AnimaEngine.AnimaShaderProgram)
            combineProgram.Create()
            combineProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-combine-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-combine-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            combineProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-combine-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-combine-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            combineProgram.Link()

            primitiveDrawProgram = shadersManager.CreateProgram("primitive-draw")
            assert isinstance(primitiveDrawProgram, AnimaEngine.AnimaShaderProgram)
            primitiveDrawProgram.Create()
            primitiveDrawProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-draw-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Primitive/primitive-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            primitiveDrawProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-draw-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Primitive/primitive-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            primitiveDrawProgram.Link()

            primitiveCombineProgram = shadersManager.CreateProgram("primitive-combine")
            assert isinstance(primitiveCombineProgram, AnimaEngine.AnimaShaderProgram)
            primitiveCombineProgram.Create()
            primitiveCombineProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-combine-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Primitive/combine-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            primitiveCombineProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-combine-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Primitive/combine-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            primitiveCombineProgram.Link()

            fxaaFilterProgram = shadersManager.CreateProgram("fxaaFilter")
            assert isinstance(fxaaFilterProgram, AnimaEngine.AnimaShaderProgram)
            fxaaFilterProgram.Create()
            fxaaFilterProgram.AddShader(shadersManager.LoadShaderFromFile("fxaaFilter-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Filters/fxaaFilter-vs.glsl", AnimaEngine.AnimaShader.VERTEX))
            fxaaFilterProgram.AddShader(shadersManager.LoadShaderFromFile("fxaaFilter-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Filters/fxaaFilter-fs.glsl", AnimaEngine.AnimaShader.FRAGMENT))
            fxaaFilterProgram.Link()

            #/Users/marco/Documents/Progetti/Repository

            self.init = True

    def SetViewport(self):
        size = self.GetClientSize()

        self.renderer.InitRenderingTargets(size.width, size.height)
        self.renderer.InitRenderingUtilities(size.width, size.height)

        self.initRenderer = True

    def OnDraw(self):
        if self.init == True and self.initRenderer == True:
            self.renderer.Start(self.scene)
            #self.renderer.DrawAll()