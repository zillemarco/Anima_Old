__author__ = 'Marco'

import wx
import details
import AnimaEngine
import threading

class AnimaGraphicsCanvas(wx.Window):
    def __init__(self, parent, engine):
        wx.Window.__init__(self, parent, style = wx.SIMPLE_BORDER | wx.FULL_REPAINT_ON_RESIZE)

        assert isinstance(engine, AnimaEngine.AnimaEngine)

        self.openGLInitialized = False
        self.rendererInitialized = False
        self.context = None
        self.engine = engine
        self.scene = None
        self.renderer = None # il renderer lo devo creare dopo aver creato il contesto
        self.keyboardInteractor = AnimaEngine.AnimaDefaultKeyboardInteractor()
        self.mouseInteractor = AnimaEngine.AnimaDefaultMouseInteractor()

        self.keyboardInteractor.Install(self.GetHandle(), self.engine)
        self.mouseInteractor.Install(self.GetHandle(), self.engine)

        self.Bind(wx.EVT_ERASE_BACKGROUND, self.OnEraseBackground)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_MOUSE_EVENTS, self.OnMouseEvent)

    def __del__(self):
        del self.renderer

    def OnEraseBackground(self, event):
        pass

    def OnSize(self, event):
        wx.CallAfter(self.SetViewport)
        event.Skip()

    def OnMouseEvent(self, event):
        if event.LeftUp():
            self.Refresh()

    def OnPaint(self, event):

        if not self.openGLInitialized:
            self.InitializeOpenGL()

        self.context.MakeCurrent()
        self.OnDraw()
        self.context.SwapBuffers()

    def InitializeOpenGL(self):
        if self.context is None:
            fbConfig = AnimaEngine.AnimaGC.GetDefaultFrameBufferConfig()
            ctConfig = AnimaEngine.AnimaGC.GetDefaultContextConfig()

            windowId = self.GetHandle()
            self.context = AnimaEngine.AnimaGC.CreateContext(windowId, ctConfig, fbConfig, False)
            assert isinstance(self.context, AnimaEngine.AnimaGC)

            self.context.MakeCurrent()

            # solo dopo aver creato il contesto e reso attivo posso creare il renderer
            self.renderer = AnimaEngine.AnimaRenderer(self.engine, self.engine.GetGenericAllocator())
            self.openGLInitialized = True

        self.SetViewport()

    def SetViewport(self):
        if self.openGLInitialized:
            size = self.GetClientSize()

            if self.scene is not None:
                vertex = AnimaEngine.AnimaVertex2f(size.width, size.height)
                self.scene.GetCamerasManager().UpdatePerspectiveCameras(90.0, vertex, 0.1, 10000.0)
                self.renderer.CheckPrograms(self.scene)

            self.renderer.InitRenderingTargets(size.width, size.height)
            self.renderer.InitRenderingUtilities(size.width, size.height)

            self.rendererInitialized = True

    def Redraw(self):
        self.Refresh()

    def SetScene(self, scene):
        self.scene = scene

        if scene is not None:
            self.scene.SetKeyboardInteractor(self.keyboardInteractor)
            self.scene.SetMouseInteractor(self.mouseInteractor)

            if self.renderer is not None:
                self.renderer.CheckPrograms(self.scene)

    def OnDraw(self):
        if self.openGLInitialized == True and self.rendererInitialized == True and self.scene is not None:
            if not self.scene.IsRunning():
                self.scene.StartScene()

            self.renderer.Start(self.scene)
            self.renderer.Render()

            self.scene.StepScene()

        threading.Timer(0.0001, self.Redraw).start()