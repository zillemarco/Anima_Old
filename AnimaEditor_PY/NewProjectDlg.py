__author__ = 'marco'

import wx

class NewProjectDlg(wx.Dialog):
    def __init__(self, *args, **kwargs):
        super(NewProjectDlg, self).__init__(*args, **kwargs)

        self.InitUI()
        self.SetSize(350, 120)

    def InitUI(self):

        panel = wx.Panel(self)

        labelProjectName = wx.StaticText(panel, wx.ID_ANY, "Project name:")
        textProjectName = wx.TextCtrl(panel, wx.ID_ANY, "")
        textProjectName.SetMinSize((200, -1))

        labelProjectPath = wx.StaticText(panel, wx.ID_ANY, "Project path:")
        textProjectPath = wx.TextCtrl(panel, wx.ID_ANY, "")
        textProjectPath.SetMinSize((200, -1))

        btnOk = wx.Button(panel, wx.ID_ANY, "Ok")
        btnCancel = wx.Button(panel, wx.ID_ANY, "Cancel")
        btnFind = wx.Button(panel, wx.ID_ANY, "...")
        btnFind.SetSizeHints(30, -1, 30, -1)

        self.Bind(wx.EVT_BUTTON, self.OnOk, btnOk)
        self.Bind(wx.EVT_BUTTON, self.OnCancel, btnCancel)

        topSizer = wx.BoxSizer(wx.VERTICAL)
        btnSizer = wx.BoxSizer(wx.HORIZONTAL)
        gridSizer = wx.GridBagSizer(hgap = 5, vgap = 5)

        gridSizer.Add(labelProjectName, pos = (0, 0), flag = wx.ALIGN_LEFT | wx.ALIGN_CENTER_VERTICAL, border = 5)
        gridSizer.Add(textProjectName, pos = (0, 1), flag = wx.EXPAND | wx.ALIGN_CENTER_VERTICAL, border = 5, span = (1, 2))
        gridSizer.Add(labelProjectPath, pos = (1, 0), flag = wx.ALIGN_LEFT | wx.ALIGN_CENTER_VERTICAL, border = 5)
        gridSizer.Add(textProjectPath, pos = (1, 1), flag = wx.EXPAND | wx.ALIGN_CENTER_VERTICAL, border = 5)
        gridSizer.Add(btnFind, pos = (1, 2), flag = wx.CENTER | wx.ALIGN_CENTER_VERTICAL, border = 5)

        gridSizer.AddGrowableCol(1)

        btnSizer.Add(btnOk, 0, wx.ALL, 5)
        btnSizer.Add(btnCancel, 0, wx.ALL, 5)

        topSizer.Add(gridSizer, 0, wx.ALL | wx.EXPAND, 5)
        topSizer.Add(btnSizer, 0, wx.ALL | wx.CENTER, 5)

        panel.SetSizer(topSizer)
        topSizer.Fit(self)

    def OnCancel(self, e):
        self.Destroy()

    def OnOk(self, e):
        self.Destroy()
