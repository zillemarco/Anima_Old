__author__ = 'marco'

import wx
import AnimaEditor
import AnimaGraphicsCanvas
import AnimaEngine
import os
import SceneDataCtrl

class MainFrame(wx.Frame):
    def __init__(self, title):
        wx.Frame.__init__(self, None, title = title, pos = (5, 25), size = (1270, 768))

        # Inizializzo l'engine che e' univoco e si trova nell'app
        assert isinstance(AnimaEditor.AnimaEditorApp.engine, AnimaEngine.AnimaEngine)
        AnimaEditor.AnimaEditorApp.engine.Initialize()

        # Definisco i controlli che poi vengono inizializzati all'interno di InitUI
        self.graphicsCtrl = None
        self.listaScene = None
        self.sceneDataCtrl = None
        self.currentScene = None

        self.InitUI()

    def InitUI(self):
        self.Bind(wx.EVT_CLOSE, self.OnClose)

        # Creazione menu File
        fileMenu = wx.Menu()
        menuNewScene = fileMenu.Append(wx.ID_NEW, "&New scene\tCtrl-N", "Creates a new scene")
        menuOpenScene = fileMenu.Append(wx.ID_OPEN, "&Open scene\tCtrl-O", "Opens a scene")
        menuSaveScene = fileMenu.Append(wx.ID_SAVE, "Save &scene\tCtrl-S", "Saves the current scene")
        menuSaveAllScenes = fileMenu.Append(wx.ID_SAVEAS, "Save all\tCtrl-Shift-A", "Saves all the loaded scenes")

        # Creazione menu Scene
        sceneMenu = wx.Menu()
        menuLoadAsset = sceneMenu.Append(wx.ID_ADD, "Load asset", "Loads an asset")

        # Creazione barra del menu
        menuBar = wx.MenuBar()
        menuBar.Append(fileMenu, "File")
        menuBar.Append(sceneMenu, "Scene")

        # Imposto la barra del menu
        self.SetMenuBar(menuBar)

        # associazione messaggi del menu
        self.Bind(wx.EVT_MENU, self.OnNewScene, menuNewScene)
        self.Bind(wx.EVT_MENU, self.OnOpenScene, menuOpenScene)
        self.Bind(wx.EVT_MENU, self.OnSaveScene, menuSaveScene)
        self.Bind(wx.EVT_MENU, self.OnSaveAllScenes, menuSaveAllScenes)
        self.Bind(wx.EVT_MENU, self.OnLoadAsset, menuLoadAsset)

        mainPanel = wx.Panel(self, wx.ID_ANY)
        leftPanel = wx.Panel(mainPanel, wx.ID_ANY)

        self.graphicsCtrl = AnimaGraphicsCanvas.AnimaGraphicsCanvas(mainPanel, AnimaEditor.AnimaEditorApp.engine)

        self.listaScene = wx.ListCtrl(leftPanel, size=(-1, 100), style=wx.LC_REPORT)
        self.sceneDataCtrl = SceneDataCtrl.SceneDataCtrl(leftPanel)

        leftSizer = wx.BoxSizer(wx.VERTICAL)
        leftSizer.Add(self.listaScene, 1, wx.EXPAND)
        leftSizer.Add(self.sceneDataCtrl, 1, wx.EXPAND)
        leftPanel.SetSizer(leftSizer)

        mainSizer = wx.BoxSizer(wx.HORIZONTAL)
        mainSizer.Add(leftPanel, 1, wx.EXPAND)
        mainSizer.Add(self.graphicsCtrl, 2, wx.EXPAND)

        mainPanel.SetSizer(mainSizer)

        self.listaScene.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnScenaSelezionata)
        self.listaScene.InsertColumn(col=0, heading="Scene name", width=200)
        self.LoadScenesList()

    def OnClose(self, event):
        self.Destroy()

    def OnNewScene(self, event):
        dlg = wx.TextEntryDialog(self, 'Insert the new scene name', 'New scene', '<new_scene>')
        if dlg.ShowModal() == wx.ID_OK:

            sceneName = dlg.GetValue().encode('utf-8')

            scenesManager = AnimaEditor.AnimaEditorApp.engine.GetScenesManager()
            newScene = scenesManager.CreateScene(sceneName)
            if newScene is not None:

                # creo di default una telecamera e una luce direzionale e imposto la texture di ambiente
                camerasManager = newScene.GetCamerasManager()
                lightsManager = newScene.GetLightsManager()
                texturesManager = newScene.GetTexturesManager()

                newLight = lightsManager.CreateDirectionalLight("default-light")
                newCamera = camerasManager.CreateCamera("default-camera")

                texturesPath = "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngineDemo/Scene/textures"

                textureCube = texturesManager.LoadTextureFromDDSFile(texturesPath + "/Roma/cubemap.dds", "dds-skybox-texture")
                newScene.SetTexture("SkyBox", textureCube)
                newScene.SetTexture("EnvironmentMap", textureCube)

                textureIrr = texturesManager.LoadTextureFromDDSFile(texturesPath + "/Roma/Irradiance.dds", "dds-skybox-texture-irr")
                newScene.SetTexture("IrradianceMap", textureIrr)

                if newLight is None:
                    print "Error creating the default light"
                else:
                    newLight.SetDirection(1, -1, -1)
                    newLight.SetColor(1, 1, 1)
                    newLight.SetIntensity(1)

                if newCamera is None:
                    print "Error creating the default camera"
                else:
                    #newCamera.SetPosition(0, 20, 30)
                    position = AnimaEngine.AnimaVertex3f(-35, 10, 15)
                    target = AnimaEngine.AnimaVertex3f(-34, 10.2, 15)
                    up = AnimaEngine.AnimaVertex3f(0, 1, 0)
                    newCamera.LookAt(position, target, up)
                    newCamera.Activate()

                self.LoadScenesList()
            else:
                print "Unable to create the new scene.\nA scene named \'" + sceneName + "\' already exists"

        dlg.Destroy()

    def OnOpenScene(self, event):

        dialog = wx.FileDialog(None, "", os.getcwd(), "", "Anima scene file (*.ascene)|*.ascene", wx.OPEN)
        if dialog.ShowModal() == wx.ID_OK:
            scenesManager = AnimaEditor.AnimaEditorApp.engine.GetScenesManager()
            if not scenesManager.LoadSceneFromFile(dialog.GetPath().encode('utf-8')):
                print "Error reading scene file"
            self.LoadScenesList()
        dialog.Destroy()

    def OnSaveScene(self, event):
        if self.currentScene is not None:
            dialog = wx.FileDialog(None, "", os.getcwd(), self.currentScene.GetName() + ".ascene", "Anima scene file (*.ascene)|*.ascene", wx.SAVE)
            if dialog.ShowModal() == wx.ID_OK:
                self.currentScene.SaveObject(dialog.GetPath().encode('utf-8'), "", False)
            dialog.Destroy()

    def OnSaveAllScenes(self, event):
        self.LoadScenesList()

    def OnLoadAsset(self, event):

        if self.currentScene is None:
            print "You need to have an active scene to load an asset"
            return

        dlgAssetName = wx.TextEntryDialog(self, 'Insert the new asset name', 'Load asset', '<new_asset>')
        if dlgAssetName.ShowModal() == wx.ID_OK:

            assetName = dlgAssetName.GetValue().encode('utf-8')

            openFileDialog = wx.FileDialog(None, "", os.getcwd(), "", "", wx.OPEN)
            if openFileDialog.ShowModal() == wx.ID_OK:
                nodesManager = self.currentScene.GetNodesManager()

                newAsset = nodesManager.LoadAssetFromExternalFile(openFileDialog.GetPath().encode('utf-8'), assetName)

                if newAsset is None:
                    print "Error reading asset"
                else:
                    nodeInstancesManager = self.currentScene.GetNodeInstancesManager()
                    assetInstance = nodeInstancesManager.CreateAssetInstance((assetName + "-instance").encode('utf-8'), newAsset)

                    assetInstance.GetTransformation().SetScale(0.01, 0.01, 0.01)
                    assetInstance.GetTransformation().TranslateY(20)
                    assetInstance.GetTransformation().RotateXDeg(-90)

                self.sceneDataCtrl.UpdateLists()
                self.graphicsCtrl.CheckPrograms()

            openFileDialog.Destroy()

        dlgAssetName.Destroy()

    def LoadScenesList(self):
        self.listaScene.DeleteAllItems()

        scenesManager = AnimaEditor.AnimaEditorApp.engine.GetScenesManager()
        count = scenesManager.GetScenesCount()

        for ns in range(0, count):
            scena = scenesManager.GetScene(ns)
            self.listaScene.InsertStringItem(ns, scena.GetName())

    def OnScenaSelezionata(self, event):
        scenesManager = AnimaEditor.AnimaEditorApp.engine.GetScenesManager()
        scena = scenesManager.GetScene(event.m_itemIndex)
        self.currentScene = scena
        self.graphicsCtrl.SetScene(self.currentScene)
        self.sceneDataCtrl.SetScene(self.currentScene)