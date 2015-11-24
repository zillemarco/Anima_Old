__author__ = 'marco'

import wx
import AnimaEditor
import AnimaGraphicsCanvas
import AnimaEngine
import os
import MaterialEditorDialog

class MainFrame(wx.Frame):
    def __init__(self, title):
        wx.Frame.__init__(self, None, title = title, pos = (0, 20), size = (1024, 768))

        # Inizializzo l'engine che e' univoco e si trova nell'app
        assert isinstance(AnimaEditor.AnimaEditorApp.engine, AnimaEngine.AnimaEngine)
        AnimaEditor.AnimaEditorApp.engine.Initialize()

        # Definisco i controlli che poi vengono inizializzati all'interno di InitUI
        self.graphicsCtrl = None
        self.listaScene = None
        self.listaMateriali = None
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

        # Creazione barra del menu
        menuBar = wx.MenuBar()
        menuBar.Append(fileMenu, "&File")

        # Imposto la barra del menu
        self.SetMenuBar(menuBar)

        # associazione messaggi del menu
        self.Bind(wx.EVT_MENU, self.OnNewScene, menuNewScene)
        self.Bind(wx.EVT_MENU, self.OnOpenScene, menuOpenScene)
        self.Bind(wx.EVT_MENU, self.OnSaveScene, menuSaveScene)
        self.Bind(wx.EVT_MENU, self.OnSaveAllScenes, menuSaveAllScenes)

        mainPanel = wx.Panel(self, wx.ID_ANY)
        leftPanel = wx.Panel(mainPanel, wx.ID_ANY)

        self.graphicsCtrl = AnimaGraphicsCanvas.AnimaGraphicsCanvas(mainPanel, AnimaEditor.AnimaEditorApp.engine)

        self.listaScene = wx.ListCtrl(leftPanel, size=(-1, 100), style=wx.LC_REPORT)
        self.listaMateriali = wx.ListCtrl(leftPanel, size=(-1, 100), style=wx.LC_REPORT)

        self.listaScene.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnScenaSelezionata)
        self.listaMateriali.Bind(wx.EVT_LIST_ITEM_ACTIVATED, self.OnMaterialeSelezionato)

        leftSizer = wx.BoxSizer(wx.VERTICAL)
        leftSizer.Add(self.listaScene, 1, wx.EXPAND | wx.ALL)
        leftSizer.Add(self.listaMateriali, 1, wx.EXPAND | wx.ALL)
        leftPanel.SetSizer(leftSizer)

        mainSizer = wx.BoxSizer(wx.HORIZONTAL)
        mainSizer.Add(leftPanel, 1, wx.EXPAND | wx.ALL)
        mainSizer.Add(self.graphicsCtrl, 2, wx.EXPAND | wx.ALL)

        mainPanel.SetSizer(mainSizer)

        self.listaScene.InsertColumn(col=0, heading="Scene name", width=200)
        self.listaMateriali.InsertColumn(col=0, heading="Material name", width=200)

        self.LoadScenesList()

    def OnClose(self, event):
        self.Destroy()

    def OnNewScene(self, event):
        self.LoadScenesList()

    def OnOpenScene(self, event):

        dialog = wx.FileDialog(None, "", os.getcwd(), "", "Anima scene file (*.ascene)|*.ascene", wx.OPEN)
        if dialog.ShowModal() == wx.ID_OK:
            scenesManager = AnimaEditor.AnimaEditorApp.engine.GetScenesManager()
            if not scenesManager.LoadSceneFromFile(dialog.GetPath().encode('utf-8')):
                print "Error reading scene file"
            self.LoadScenesList()
        dialog.Destroy()

    def OnSaveScene(self, event):
        self.LoadScenesList()

    def OnSaveAllScenes(self, event):
        self.LoadScenesList()

    def LoadScenesList(self):
        self.listaScene.DeleteAllItems()

        scenesManager = AnimaEditor.AnimaEditorApp.engine.GetScenesManager()
        count = scenesManager.GetScenesCount()

        for ns in range(0, count):
            scena = scenesManager.GetScene(ns)
            self.listaScene.InsertStringItem(ns, scena.GetName())

    def LoadMaterialsList(self):
        self.listaMateriali.DeleteAllItems()
        if self.currentScene is not None:
            materialsManager = self.currentScene.GetMaterialsManager()
            count = materialsManager.GetMaterialsCount()

            for nm in range(0, count):
                material = materialsManager.GetMaterial(nm)
                self.listaMateriali.InsertStringItem(nm, material.GetName())

    def OnScenaSelezionata(self, event):
        scenesManager = AnimaEditor.AnimaEditorApp.engine.GetScenesManager()
        scena = scenesManager.GetScene(event.m_itemIndex)
        self.currentScene = scena
        self.graphicsCtrl.SetScene(self.currentScene)

        self.LoadMaterialsList()

    def OnMaterialeSelezionato(self, event):
        if self.currentScene is not None:
            materialsManager = self.currentScene.GetMaterialsManager()
            materiale = materialsManager.GetMaterial(event.m_itemIndex)

            dialog = MaterialEditorDialog.MaterialEditorDialog(self)
            dialog.SetData(self.currentScene, materiale)
            dialog.ShowModal()

