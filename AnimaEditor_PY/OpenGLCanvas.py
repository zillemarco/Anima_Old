__author__ = 'Marco'

import wx
import details
import AnimaEngine
import threading

class OpenGLCanvas(wx.Window):
    def __init__(self, parent):
        wx.Window.__init__(self, parent, style = wx.SIMPLE_BORDER | wx.FULL_REPAINT_ON_RESIZE)

        #details.describe(AnimaEngine)

        self.ondrawcalls = 0

        self.init = False
        self.initRenderer = False

        fbConfig = AnimaEngine.AnimaGC.GetDefaultFrameBufferConfig()
        ctConfig = AnimaEngine.AnimaGC.GetDefaultContextConfig()

        windowId = self.GetHandle()

        self.context = AnimaEngine.AnimaGC.CreateContext(windowId, ctConfig, fbConfig)
        assert isinstance(self.context, AnimaEngine.AnimaGC)

        self.engine = AnimaEngine.AnimaEngine();
        assert isinstance(self.engine, AnimaEngine.AnimaEngine)
        self.engine.Initialize()

        scenesManager = self.engine.GetScenesManager()
        assert isinstance(scenesManager, AnimaEngine.AnimaScenesManager)

        self.scene = scenesManager.CreateScene("test-scene")
        assert isinstance(self.scene, AnimaEngine.AnimaScene)

        modelsManager = self.scene.GetModelsManager()
        assert isinstance(modelsManager, AnimaEngine.AnimaModelsManager)

        #self.model = modelsManager.LoadModel("D:/Git/Anima/AnimaEngine/data/models/cubo.3ds", "cubo")
        self.model = modelsManager.LoadModel("C:/Users/Marco/Desktop/Model/Model_MR.dae", "uomo")
        assert isinstance(self.model, AnimaEngine.AnimaModel)

        self.camerasManager = self.scene.GetCamerasManager()
        assert isinstance(self.camerasManager, AnimaEngine.AnimaCamerasManager)
        self.camera = self.camerasManager.CreateFirstPersonCamera("camera")
        assert isinstance(self.camera, AnimaEngine.AnimaFirstPersonCamera)
        self.camera.LookAt(0.0, 90.0, 200.0, 0.0, 0.0, -1.0)
        self.camera.Activate()

        self.animationsManager = self.scene.GetAnimationsManager()
        assert isinstance(self.animationsManager, AnimaEngine.AnimaAnimationsManager)

        self.timer = AnimaEngine.AnimaTimer()
        assert isinstance(self.timer, AnimaEngine.AnimaTimer)

        self.renderer = AnimaEngine.AnimaRenderer(self.engine, self.engine.GetGenericAllocator())

        self.Bind(wx.EVT_ERASE_BACKGROUND, self.OnEraseBackground)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_MOUSE_EVENTS, self.OnMouseEvent)

    def __del__(self):
        del self.renderer
        self.engine.Terminate()

    def OnEraseBackground(self, event):
        pass

    def OnSize(self, event):
        wx.CallAfter(self.SetViewport)
        event.Skip()

    def OnMouseEvent(self, event):
        if event.LeftUp():
            self.Refresh()

    def OnPaint(self, event):
        self.context.MakeCurrent()

        if not self.init:
            self.InitGl()

        self.OnDraw()
        self.context.SwapBuffers()

    def InitGl(self):
        result = AnimaEngine.AnimaGC.InitializeGLEWExtensions()
        if result:
            assert isinstance(self.scene, AnimaEngine.AnimaScene)
            shadersManager = self.scene.GetShadersManager()
            assert isinstance(shadersManager, AnimaEngine.AnimaShadersManager)

            prepareProgram = shadersManager.CreateProgram("deferred-prepare")
            assert isinstance(prepareProgram, AnimaEngine.AnimaShaderProgram)
            prepareProgram.Create()
            prepareProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-prepare-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-prepare-animated-vs.glsl", AnimaEngine.AnimaShaderType.VERTEX))
            prepareProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-prepare-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-prepare-fs.glsl", AnimaEngine.AnimaShaderType.FRAGMENT))
            prepareProgram.Link()

            shadowProgram = shadersManager.CreateProgram("deferred-shadowMap")
            assert isinstance(shadowProgram, AnimaEngine.AnimaShaderProgram)
            shadowProgram.Create()
            shadowProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-shadowMap-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-shadowMap-vs.glsl", AnimaEngine.AnimaShaderType.VERTEX))
            shadowProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-shadowMap-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-shadowMap-fs.glsl", AnimaEngine.AnimaShaderType.FRAGMENT))
            shadowProgram.Link()

            combineProgram = shadersManager.CreateProgram("deferred-combine")
            assert isinstance(combineProgram, AnimaEngine.AnimaShaderProgram)
            combineProgram.Create()
            combineProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-combine-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-combine-vs.glsl", AnimaEngine.AnimaShaderType.VERTEX))
            combineProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-combine-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-combine-fs.glsl", AnimaEngine.AnimaShaderType.FRAGMENT))
            combineProgram.Link()

            primitiveDrawProgram = shadersManager.CreateProgram("primitive-draw")
            assert isinstance(primitiveDrawProgram, AnimaEngine.AnimaShaderProgram)
            primitiveDrawProgram.Create()
            primitiveDrawProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-draw-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Primitive/primitive-vs.glsl", AnimaEngine.AnimaShaderType.VERTEX))
            primitiveDrawProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-draw-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Primitive/primitive-fs.glsl", AnimaEngine.AnimaShaderType.FRAGMENT))
            primitiveDrawProgram.Link()

            primitiveCombineProgram = shadersManager.CreateProgram("primitive-combine")
            assert isinstance(primitiveCombineProgram, AnimaEngine.AnimaShaderProgram)
            primitiveCombineProgram.Create()
            primitiveCombineProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-combine-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Primitive/combine-vs.glsl", AnimaEngine.AnimaShaderType.VERTEX))
            primitiveCombineProgram.AddShader(shadersManager.LoadShaderFromFile("primitive-combine-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Primitive/combine-fs.glsl", AnimaEngine.AnimaShaderType.FRAGMENT))
            primitiveCombineProgram.Link()

            fxaaFilterProgram = shadersManager.CreateProgram("fxaaFilter")
            assert isinstance(fxaaFilterProgram, AnimaEngine.AnimaShaderProgram)
            fxaaFilterProgram.Create()
            fxaaFilterProgram.AddShader(shadersManager.LoadShaderFromFile("fxaaFilter-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Filters/fxaaFilter-vs.glsl", AnimaEngine.AnimaShaderType.VERTEX))
            fxaaFilterProgram.AddShader(shadersManager.LoadShaderFromFile("fxaaFilter-fs", "D:/Git/Anima/AnimaEngine/data/shaders/Filters/fxaaFilter-fs.glsl", AnimaEngine.AnimaShaderType.FRAGMENT))
            fxaaFilterProgram.Link()

            #/Users/marco/Documents/Progetti/Repository
            self.timer.Reset()

            self.init = True

    def SetViewport(self):
        size = self.GetClientSize()
        vertex = AnimaEngine.AnimaVertex2f(size.width, size.height)
        self.camerasManager.UpdatePerspectiveCameras(60.0, vertex, 0.1, 10000.0)

        self.renderer.InitRenderingTargets(size.width, size.height)
        self.renderer.InitRenderingUtilities(size.width, size.height)

        self.initRenderer = True

    def Redraw(self):
        self.Refresh()
        #self.Update()

    def OnDraw(self):
        if self.init == True and self.initRenderer == True:
            if self.model.GetAnimationsCount > 0:
                animation = self.model.GetAnimation(0)
                assert isinstance(animation, AnimaEngine.AnimaAnimation)
                animation.UpdateAnimation(self.model, self.timer.Elapsed())
            self.renderer.Start(self.scene)
            self.renderer.DrawModel(self.model)
        threading.Timer(0.0001, self.Redraw).start()