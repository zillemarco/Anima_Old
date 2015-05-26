__author__ = 'marco'

import tkinter as tk    # importo il modulo Tkinter per gestire l'app ed i widget
import MainFrame        # importo il file MainFrame che contiene la classe MainFrame
import AnimaEngine as AE
import GCControl as GC

# creo la classe AnimaEditor e la faccio derivare da tk.Tk che è la classe App del modulo Tkinter
class AnimaEditor(tk.Tk):

    # cotruttore della classe
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)   # chiamata al costruttore della classe base
        self.initUI()                           # chiamata alla funzione di inizializzazione dell'interfaccia

    # metodo initUI
    def initUI(self):
        self.title("AnimaEditor")               # imposto il titolo della finestra dell'app
        #frame = MainFrame.MainFrame(self)       # creo una nuova istanza della classe MainFrame importata dal file MainFrame.py
        frame = GC.GCControl(self)
        frame.tkraise()                         # avvio e mostro la finestra del mainframe

def main():
    app = AnimaEditor()
    app.geometry("500x500")
    app.mainloop()

if __name__ == '__main__':
    main()