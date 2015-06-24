__author__ = 'marco'

import wx
#import AboutBox
import AnimaEditor
import Project
import NewProjectDlg
import GraphicsCtrl
import OpenGLCanvas

class MainFrame(wx.Frame):
    def __init__(self, title):
        wx.Frame.__init__(self, None, title = title, pos = (0, 20), size = (700, 700))
        self.Bind(wx.EVT_CLOSE, self.OnClose)

        menuBar = wx.MenuBar()

        fileMenu = wx.Menu()
        menuNewProject = fileMenu.Append(wx.ID_NEW, "&New project\tCtrl-N", "Creates a new project")
        menuOpenProject = fileMenu.Append(wx.ID_OPEN, "&Open project\tCtrl-O", "Opens a project")

        self.Bind(wx.EVT_MENU, self.OnNewProject, menuNewProject)
        self.Bind(wx.EVT_MENU, self.OnOpenProject, menuOpenProject)

        menuBar.Append(fileMenu, "&File")

        helpMenu = wx.Menu()
        m_about = helpMenu.Append(wx.ID_ABOUT, "&About", "Information about this program")
        self.Bind(wx.EVT_MENU, self.OnAbout, m_about)
        menuBar.Append(helpMenu, "&Help")

        self.SetMenuBar(menuBar)
        self.statusbar = self.CreateStatusBar()

        #self.graphicsCtrl = GraphicsCtrl.AnimaEditorCanvas(self)
        self.graphicsCtrl = OpenGLCanvas.OpenGLCanvas(self)

    def OnClose(self, event):
        self.Destroy()

    def OnNewProject(self, event):
        dlg = NewProjectDlg.NewProjectDlg(None, title="New project")
        dlg.ShowModal()
        dlg.Destroy()

    def OnOpenProject(self, event):

        if AnimaEditor.AnimaEditorApp.currentProject is None:
            print("Non ancora aperto")
        else:
            assert isinstance(AnimaEditor.AnimaEditorApp.currentProject, Project.Project)
            print(AnimaEditor.AnimaEditorApp.currentProject.GetName())

    def OnAbout(self, event):
        pass
        #dlg = AboutBox.AboutBox()
        #dlg.ShowModal()
        #dlg.Destroy()