__author__ = 'marco'

import wx
import wx.html

aboutText = """<strong><a href="https://github.com/zillemarco/Anima">AnimaEditor</a></strong>
            <br/><br/>
            AnimaEditor è un programma che sfrutta AnimaEngine come motore grafico e serve per caricare modelli 3D, impostare materiali e creare la struttura di un possibile gioco.
            <br/><br/>
            AnimaEditor is a program using AnimaEngine as a graphic engine and it's used to load 3D models, set materials and create the basic structure of a possible game.
            """

class AboutBox(wx.Dialog):
    def __init__(self):
        wx.Dialog.__init__(self, None, -1, "About AnimaEditor", style = wx.DEFAULT_DIALOG_STYLE | wx.TAB_TRAVERSAL)
        htmlWindow = wx.html.HtmlWindow(self, -1, size = (500, 200))
        vers = {}
        htmlWindow.SetPage(aboutText)
        btn = htmlWindow.FindWindowById(wx.ID_OK)
        irep = htmlWindow.GetInternalRepresentation()
        htmlWindow.SetSize((irep.GetWidth() + 25, irep.GetHeight() + 10))
        self.SetClientSize(htmlWindow.GetSize())
        self.CentreOnParent(wx.BOTH)
        self.SetFocus()