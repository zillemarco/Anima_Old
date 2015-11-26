__author__ = 'marco'

import wx
import MaterialEditorDialog

class MaterialsTab(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)

        self.currentScene = None
        self.listaMateriali = wx.ListCtrl(self, size=(-1, -1), style=wx.LC_REPORT)
        self.listaMateriali.InsertColumn(col=0, heading="Material name", width=200)
        self.listaMateriali.Bind(wx.EVT_LIST_ITEM_ACTIVATED, self.OnMaterialeSelezionato)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.listaMateriali, 1, wx.EXPAND)

        self.SetSizer(sizer)

    def OnMaterialeSelezionato(self, event):
        if self.currentScene is not None:
            materialsManager = self.currentScene.GetMaterialsManager()
            materiale = materialsManager.GetMaterial(event.m_itemIndex)

            dialog = MaterialEditorDialog.MaterialEditorDialog(self)
            dialog.SetData(self.currentScene, materiale)
            dialog.ShowModal()

    def LoadMaterialsList(self):
        self.listaMateriali.DeleteAllItems()
        if self.currentScene is not None:
            materialsManager = self.currentScene.GetMaterialsManager()
            count = materialsManager.GetMaterialsCount()

            for nm in range(0, count):
                material = materialsManager.GetMaterial(nm)
                self.listaMateriali.InsertStringItem(nm, material.GetName())

    def SetScene(self, scene):
        self.currentScene = scene
        self.LoadMaterialsList()

class TexturesTab(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)

        self.currentScene = None
        self.listaTexture = wx.ListCtrl(self, size=(-1, -1), style=wx.LC_REPORT)
        self.listaTexture.InsertColumn(col=0, heading="Texture name", width=200)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.listaTexture, 1, wx.EXPAND)

        self.SetSizer(sizer)

    def LoadTexturesList(self):
        self.listaTexture.DeleteAllItems()
        if self.currentScene is not None:
            texturesManager = self.currentScene.GetTexturesManager()
            count = texturesManager.GetTexturesCount()

            for nt in range(0, count):
                texture = texturesManager.GetTexture(nt)
                self.listaTexture.InsertStringItem(nt, texture.GetName())

    def SetScene(self, scene):
        self.currentScene = scene
        self.LoadTexturesList()

class GeometriesTab(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)

        self.currentScene = None
        self.listaGeometrie = wx.ListCtrl(self, size=(-1, -1), style=wx.LC_REPORT)
        self.listaGeometrie.InsertColumn(col=0, heading="Geometry name", width=200)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.listaGeometrie, 1, wx.EXPAND)

        self.SetSizer(sizer)

    def LoadGeometriesList(self):
        self.listaGeometrie.DeleteAllItems()
        if self.currentScene is not None:
            geometriesManager = self.currentScene.GetGeometriesManager()
            count = geometriesManager.GetGeometriesCount()

            for ng in range(0, count):
                geometry = geometriesManager.GetGeometry(ng)
                self.listaGeometrie.InsertStringItem(ng, geometry.GetName())

    def SetScene(self, scene):
        self.currentScene = scene
        self.LoadGeometriesList()

class AssetsTab(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)

        self.currentScene = None
        self.listaAsset = wx.ListCtrl(self, size=(-1, -1), style=wx.LC_REPORT)
        self.listaAsset.InsertColumn(col=0, heading="Asset name", width=200)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.listaAsset, 1, wx.EXPAND)

        self.SetSizer(sizer)

    def LoadAssetsList(self):
        self.listaAsset.DeleteAllItems()
        if self.currentScene is not None:
            nodesManager = self.currentScene.GetNodesManager()
            count = nodesManager.GetAssetsCount()

            for na in range(0, count):
                asset = nodesManager.GetAsset(na)
                self.listaAsset.InsertStringItem(na, asset.GetName())

    def SetScene(self, scene):
        self.currentScene = scene
        self.LoadAssetsList()

class CamerasTab(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)

        self.currentScene = None
        self.listaCamere = wx.ListCtrl(self, size=(-1, -1), style=wx.LC_REPORT)
        self.listaCamere.InsertColumn(col=0, heading="Camera name", width=200)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.listaCamere, 1, wx.EXPAND)

        self.SetSizer(sizer)

    def LoadCamerasList(self):
        self.listaCamere.DeleteAllItems()
        if self.currentScene is not None:
            camerasManager = self.currentScene.GetCamerasManager()
            count = camerasManager.GetCamerasCount()

            for nc in range(0, count):
                camera = camerasManager.GetCamera(nc)
                self.listaCamere.InsertStringItem(nc, camera.GetName())

    def SetScene(self, scene):
        self.currentScene = scene
        self.LoadCamerasList()

class SceneDataCtrl(wx.Notebook):
    def __init__(self, parent):
        wx.Notebook.__init__(self, parent, id=wx.ID_ANY, style=wx.BK_DEFAULT)

        self.currentScene = None

        self.tabAssets = AssetsTab(self)
        self.tabAssets.SetBackgroundColour("Gray")
        self.AddPage(self.tabAssets, "Assets")

        self.tabGeometrie = GeometriesTab(self)
        self.tabGeometrie.SetBackgroundColour("Gray")
        self.AddPage(self.tabGeometrie, "Geometries")

        self.tabCamere = CamerasTab(self)
        self.tabCamere.SetBackgroundColour("Gray")
        self.AddPage(self.tabCamere, "Cameras")

        self.tabMateriali = MaterialsTab(self)
        self.tabMateriali.SetBackgroundColour("Gray")
        self.AddPage(self.tabMateriali, "Materials")

        self.tabTexture = TexturesTab(self)
        self.tabTexture.SetBackgroundColour("Gray")
        self.AddPage(self.tabTexture, "Textures")

    def SetScene(self, scene):
        self.currentScene = scene
        self.tabAssets.SetScene(self.currentScene)
        self.tabGeometrie.SetScene(self.currentScene)
        self.tabCamere.SetScene(self.currentScene)
        self.tabMateriali.SetScene(self.currentScene)
        self.tabTexture.SetScene(self.currentScene)

    def UpdateLists(self):
        self.tabAssets.LoadAssetsList()
        self.tabGeometrie.LoadGeometriesList()
        self.tabCamere.LoadCamerasList()
        self.tabMateriali.LoadMaterialsList()
        self.tabTexture.LoadTexturesList()