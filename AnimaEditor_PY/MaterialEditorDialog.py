__author__ = 'marco'

import wx

class MaterialEditorDialog(wx.Dialog):
    def __init__(self, *args, **kwargs):
        super(MaterialEditorDialog, self).__init__(*args, **kwargs)

        self.material = None
        self.scene = None
        self.listaMateriali = None

        self.InitUI()

        self.LoadMaterialData()

    def SetData(self, scene, material):
        self.material = material
        self.scene = scene

        self.LoadMaterialData()

    def InitUI(self):
        self.listaMateriali = wx.ListCtrl(self, style=wx.LC_REPORT)

        self.listaMateriali.InsertColumn(col=0, heading="Property", width=120)
        self.listaMateriali.InsertColumn(col=1, heading="Value", width=120)

    def LoadMaterialData(self):
        if self.material is not None:
            floats = self.material.GetFloats()
            colors = self.material.GetColors()

            floatsCount = len(floats)
            colorsCount = len(colors)

            for k in floats:
                print k

            pass

    def OnCancel(self, e):
        self.Destroy()

    def OnOk(self, e):
        self.Destroy()
