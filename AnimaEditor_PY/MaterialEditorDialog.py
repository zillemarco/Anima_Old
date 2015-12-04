__author__ = 'marco'

import wx
import AnimaEngine

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

            pos = 0
            for f in floats:
                self.listaMateriali.InsertStringItem(pos, AnimaEngine.AnimaMappedValues.ExtractName(f.key()))
                self.listaMateriali.SetStringItem(pos, 1, str(f.data()))
                pos += 1

            for c in colors:
                col = c.data().GetColor3f()

                self.listaMateriali.InsertStringItem(pos, AnimaEngine.AnimaMappedValues.ExtractName(c.key()))
                self.listaMateriali.SetStringItem(pos, 1, "{:.2f}:{.2f}:{.2f}".format(col.r, col.g, col.b))
                pos += 1

    def OnCancel(self, e):
        self.Destroy()

    def OnOk(self, e):
        self.Destroy()
