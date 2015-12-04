__author__ = 'marco'

import wx
import wx.lib.mixins.listctrl as listmix
import MaterialEditorDialog
import AnimaEngine

class EditableListCtrl(wx.ListCtrl, listmix.TextEditMixin):
    def __init__(self, parent, ID=wx.ID_ANY, pos=wx.DefaultPosition, size=wx.DefaultSize, style=0):
        """Constructor"""
        wx.ListCtrl.__init__(self, parent, ID, pos, size, style)
        listmix.TextEditMixin.__init__(self)

        self.parent = parent
        self.editingRow = -1
        self.editingCol = -1
        self.editedCallback = None

    def SetEditedCalback(self, callback):
        self.editedCallback = callback

    def OpenEditor(self, col, row):
        self.editingRow = row
        self.editingCol = col
        listmix.TextEditMixin.OpenEditor(self, col, row)

    def CloseEditor(self, evt=None):
        if evt is not None and isinstance(evt, wx.CommandEvent) and self.editedCallback is not None and self.editingRow >= 0 and self.editingCol >= 0:
            self.editedCallback(self.editingRow, self.editingCol, evt.String)

        self.editingRow = -1
        self.editingCol = -1

        listmix.TextEditMixin.CloseEditor(self, evt)

class MaterialsTab(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)

        self.currentScene = None
        self.currentSelectedMaterial = None

        self.listaMateriali = wx.ListCtrl(self, size=(-1, -1), style=wx.LC_REPORT)
        self.listaMateriali.InsertColumn(col=0, heading="Material name", width=200)
        self.listaMateriali.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnMaterialeSelezionato)

        self.listaProprietaMateriale = EditableListCtrl(self, size=(-1, -1), style=wx.LC_REPORT)
        self.listaProprietaMateriale.InsertColumn(col=0, heading="Property", width=100)
        self.listaProprietaMateriale.InsertColumn(col=1, heading="r", width=70)
        self.listaProprietaMateriale.InsertColumn(col=2, heading="g", width=70)
        self.listaProprietaMateriale.InsertColumn(col=3, heading="b", width=70)
        self.listaProprietaMateriale.SetEditedCalback(self.CellEditedCallback)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.listaMateriali, 1, wx.EXPAND)
        sizer.Add(self.listaProprietaMateriale, 1, wx.EXPAND)

        self.SetSizer(sizer)

    def CellEditedCallback(self, row, col, text):
        if self.currentSelectedMaterial is None:
            return

        value = float(text)

        if row == 0:
            albedo = self.currentSelectedMaterial.GetColor3f("Albedo")

            if col == 1:
                albedo.r = value
            elif col == 2:
                albedo.g = value
            elif col == 3:
                albedo.b = value

            self.currentSelectedMaterial.SetColor("Albedo", albedo)
        elif row == 1:
            self.currentSelectedMaterial.SetFloat("Metallic", value)
        elif row == 2:
            self.currentSelectedMaterial.SetFloat("Roughness", value)
        elif row == 3:
            self.currentSelectedMaterial.SetFloat("Specular", value)
        elif row == 4:
            self.currentSelectedMaterial.SetFloat("ReflectionIntensity", value)

    def OnMaterialeSelezionato(self, event):
        self.listaProprietaMateriale.DeleteAllItems()
        self.currentSelectedMaterial = None

        if self.currentScene is not None:
            materialsManager = self.currentScene.GetMaterialsManager()
            material = materialsManager.GetMaterial(event.m_itemIndex)

            albedo = material.GetColor3f("Albedo")
            reflectionIntensity = material.GetFloat("ReflectionIntensity")
            metallic = material.GetFloat("Metallic")
            roughness = material.GetFloat("Roughness")
            specular = material.GetFloat("Specular")

            self.listaProprietaMateriale.InsertStringItem(0, "Albedo")
            self.listaProprietaMateriale.SetStringItem(0, 1, "{:.3f}".format(albedo.r))
            self.listaProprietaMateriale.SetStringItem(0, 2, "{:.3f}".format(albedo.g))
            self.listaProprietaMateriale.SetStringItem(0, 3, "{:.3f}".format(albedo.b))

            self.listaProprietaMateriale.InsertStringItem(1, "Metallic")
            self.listaProprietaMateriale.SetStringItem(1, 1, "{:.3f}".format(metallic))

            self.listaProprietaMateriale.InsertStringItem(2, "Roughness")
            self.listaProprietaMateriale.SetStringItem(2, 1, "{:.3f}".format(roughness))

            self.listaProprietaMateriale.InsertStringItem(3, "Specular")
            self.listaProprietaMateriale.SetStringItem(3, 1, "{:.3f}".format(specular))

            self.listaProprietaMateriale.InsertStringItem(4, "Reflection")
            self.listaProprietaMateriale.SetStringItem(4, 1, "{:.3f}".format(reflectionIntensity))

            self.currentSelectedMaterial = material

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
        self.currentSelectedGeometry = None
        self.listaGeometrie = wx.ListCtrl(self, size=(-1, -1), style=wx.LC_REPORT)
        self.listaGeometrie.InsertColumn(col=0, heading="Geometry name", width=200)

        self.listaGeometrie.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnGeometriaSelezionata)
        self.listaGeometrie.Bind(wx.EVT_LIST_ITEM_RIGHT_CLICK, self.OnGeometryRClick)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.listaGeometrie, 1, wx.EXPAND)

        self.SetSizer(sizer)

    def OnGeometriaSelezionata(self, event):
        pass

    def LoadGeometriesList(self):
        self.listaGeometrie.DeleteAllItems()
        if self.currentScene is not None:
            geometryInstancesManager = self.currentScene.GetGeometryInstancesManager()
            count = geometryInstancesManager.GetGeometryInstancesCount()

            for ng in range(0, count):
                geometry = geometryInstancesManager.GetGeometryInstance(ng)
                self.listaGeometrie.InsertStringItem(ng, geometry.GetName())

    def OnGeometryRClick(self, event):
        geometryInstancesManager = self.currentScene.GetGeometryInstancesManager()
        self.currentSelectedGeometry = geometryInstancesManager.GetGeometryInstance(event.m_itemIndex)

        menu = wx.Menu()
        menuSetMaterial = menu.Append(wx.ID_ANY, "Set material", "Choose a material and set it to the selected geometry")
        self.Bind(wx.EVT_MENU, self.OnSetMaterialToGeometry, menuSetMaterial)

        self.PopupMenu(menu, event.GetPoint())
        menu.Destroy()

    def OnSetMaterialToGeometry(self, event):
        if self.currentSelectedGeometry is not None:
            dlgMaterialName = wx.TextEntryDialog(self, 'Insert the material name', 'Choose material', '')
            if dlgMaterialName.ShowModal() == wx.ID_OK:
                materialsManager = self.currentScene.GetMaterialsManager()
                materialName = dlgMaterialName.GetValue().encode('utf-8')
                material = materialsManager.GetMaterialFromName(materialName)

                if material is not None:
                    self.currentSelectedGeometry.SetMaterial(material)

        self.currentSelectedGeometry = None

    def SetScene(self, scene):
        self.currentScene = scene
        self.LoadGeometriesList()

class AssetsTab(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)

        self.currentScene = None
        self.currentSelectedAsset = None

        self.listaAsset = wx.ListCtrl(self, size=(-1, -1), style=wx.LC_REPORT)
        self.listaAsset.InsertColumn(col=0, heading="Asset name", width=200)
        self.listaAsset.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnAssetSelezionato)

        self.listaProprietaAsset = EditableListCtrl(self, size=(-1, -1), style=wx.LC_REPORT)
        self.listaProprietaAsset.InsertColumn(col=0, heading="Property", width=100)
        self.listaProprietaAsset.InsertColumn(col=1, heading="x", width=70)
        self.listaProprietaAsset.InsertColumn(col=2, heading="y", width=70)
        self.listaProprietaAsset.InsertColumn(col=3, heading="z", width=70)
        self.listaProprietaAsset.SetEditedCalback(self.CellEditedCallback)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.listaAsset, 1, wx.EXPAND)
        sizer.Add(self.listaProprietaAsset, 1, wx.EXPAND)

        self.SetSizer(sizer)

    def LoadAssetsList(self):
        self.listaAsset.DeleteAllItems()
        self.listaProprietaAsset.DeleteAllItems()

        if self.currentScene is not None:
            nodeInstancesManager = self.currentScene.GetNodeInstancesManager()
            count = nodeInstancesManager.GetAssetInstancesCount()

            for na in range(0, count):
                asset = nodeInstancesManager.GetAssetInstance(na)
                self.listaAsset.InsertStringItem(na, asset.GetName())

    def CellEditedCallback(self, row, col, text):
        if self.currentSelectedAsset is None:
            return

        value = float(text)
        transformation = self.currentSelectedAsset.GetTransformation()

        position = self.currentSelectedAsset.GetPosition()
        rotation = transformation.GetRotation()
        scale = transformation.GetScale()

        if row == 0: # riga della posizione
            if col == 1:
                position.x = value
            elif col == 2:
                position.y = value
            elif col == 3:
                position.z = value
        elif row == 1: # riga della scala
            if col == 1:
                scale.x = value
            elif col == 2:
                scale.y = value
            elif col == 3:
                scale.z = value
        elif row == 2: # riga della rotazione
            value = value / 180.0 * 3.1415
            if col == 1:
                rotation.x = value
            elif col == 2:
                rotation.y = value
            elif col == 3:
                rotation.z = value

        self.currentSelectedAsset.SetPosition(position)
        transformation.SetRotation(rotation)
        transformation.SetScale(scale)

    def SetScene(self, scene):
        self.currentScene = scene
        self.LoadAssetsList()

    def OnAssetSelezionato(self, event):
        self.listaProprietaAsset.DeleteAllItems()
        self.currentSelectedAsset = None

        if self.currentScene is not None:
            nodeInstancesManager = self.currentScene.GetNodeInstancesManager()
            asset = nodeInstancesManager.GetAssetInstance(event.m_itemIndex)

            transformation = asset.GetTransformation()
            position = asset.GetPosition()

            rotationX = transformation.GetRotationX() * 180.0 / 3.1415
            rotationY = transformation.GetRotationY() * 180.0 / 3.1415
            rotationZ = transformation.GetRotationZ() * 180.0 / 3.1415
            scaleX = transformation.GetScaleX()
            scaleY = transformation.GetScaleY()
            scaleZ = transformation.GetScaleZ()

            self.listaProprietaAsset.InsertStringItem(0, "Position")
            self.listaProprietaAsset.SetStringItem(0, 1, str(position.x))
            self.listaProprietaAsset.SetStringItem(0, 2, str(position.y))
            self.listaProprietaAsset.SetStringItem(0, 3, str(position.z))

            self.listaProprietaAsset.InsertStringItem(1, "Rotation")
            self.listaProprietaAsset.SetStringItem(1, 1, str(rotationX))
            self.listaProprietaAsset.SetStringItem(1, 2, str(rotationY))
            self.listaProprietaAsset.SetStringItem(1, 3, str(rotationZ))

            self.listaProprietaAsset.InsertStringItem(1, "Scale")
            self.listaProprietaAsset.SetStringItem(1, 1, str(scaleX))
            self.listaProprietaAsset.SetStringItem(1, 2, str(scaleY))
            self.listaProprietaAsset.SetStringItem(1, 3, str(scaleZ))

            self.currentSelectedAsset = asset

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