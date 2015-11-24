__author__ = 'marco'

import sys, os, inspect
AnimaEngine_Folder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0],"AnimaEngine")))
if AnimaEngine_Folder not in sys.path:
    sys.path.insert(0, AnimaEngine_Folder)

import wx
import MainFrame
import AnimaEngine

class AnimaEditorApp(wx.App):
    currentProject = None
    engine = AnimaEngine.AnimaEngine()

    def __init__(self):
        wx.App.__init__(self, redirect = False)
        self.frame = None

    def OnInit(self):
        mainFrame = MainFrame.MainFrame("Anima Editor")
        mainFrame.Show(True)
        self.SetTopWindow(mainFrame)
        self.frame = mainFrame

        return True

    def OnExitApp(self, event):
        self.frame.Close(True)

def main():
    app = AnimaEditorApp()
    app.MainLoop()

if __name__ == '__main__':
    main()