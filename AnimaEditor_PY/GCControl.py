__author__ = 'Marco'

import tkinter as tk    # importo il modulo Tkinter per gestire l'app ed i widget
import AnimaEngine as AE
import ctypes as ct

class GCControl(tk.Frame):

    def __init__(self, parent, *args, **kwargs):
        tk.Frame.__init__(self, parent)

        self.gc = None
        self.gcInitialized = False
        self.after(10, self.Paint)

    def InitGraphics(self):

        ctx = AE.AnimaGC_GetDefaultContextConfig()
        fbx = AE.AnimaGC_GetDefaultFrameBufferConfig()

        self.gc = AE.AnimaGC_CreateContext(self.winfo_id(), ctx, fbx)
        self.gcInitialized = True

    def Paint(self):

        if self.gcInitialized != True:
            self.InitGraphics()

        self.gc.MakeCurrent()
        self.gc.ClearColor(1.0, 0.0, 0.0, 0.0)
        self.gc.SwapBuffers()