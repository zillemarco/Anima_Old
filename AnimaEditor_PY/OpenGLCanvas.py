__author__ = 'Marco'

import wx
import details
import AnimaEngine

class OpenGLCanvas(wx.Window):
    def __init__(self, parent):
        wx.Window.__init__(self, parent, style = wx.SIMPLE_BORDER | wx.FULL_REPAINT_ON_RESIZE)

        details.describe(AnimaEngine)

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

        model = modelsManager.LoadModel("D:/Git/Anima/AnimaEngine/data/models/cubo.3ds", "cubo")
        assert isinstance(model, AnimaEngine.AnimaModel)

        camerasManager = self.scene.GetCamerasManager()
        assert isinstance(camerasManager, AnimaEngine.AnimaCamerasManager)
        fpc = camerasManager.CreateFirstPersonCamera("camera")
        assert isinstance(fpc, AnimaEngine.AnimaFirstPersonCamera)
        self.camera = camerasManager.GetCameraFromName("camera")
        self.camera.Activate()

        self.renderer = AnimaEngine.AnimaRenderer(self.engine, self.engine.GetGenericAllocator())

        self.Bind(wx.EVT_ERASE_BACKGROUND, self.OnEraseBackground)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_PAINT, self.OnPaint)

    def OnEraseBackground(self, event):
        pass

    def OnSize(self, event):
        wx.CallAfter(self.SetViewport)
        event.Skip()

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
            prepareProgram.AddShader(shadersManager.LoadShaderFromFile("deferred-prepare-vs", "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/deferred-prepare-vs.glsl", AnimaEngine.AnimaShaderType.VERTEX))
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