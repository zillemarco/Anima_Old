__author__ = 'marco'

import tkinter as tk                    # importo il modulo tkinter per gestire l'app ed i widget
import tkinter.ttk as ttk               # importo il modulo tkinter.ttk per poter utilizzare dei controlli con uno stile visivo migliore
import tkinter.messagebox as messagebox # importo il modulo tkinter.messagebox per poter utilizzare i messagebox
import AEUtils                          # importo il file AEUtils
import AENewProject

# creo la class MainFrame derivandola dalla classe ttk.Frame
class MainFrame(ttk.Frame):

    # costruttore della classe
    def __init__(self, parent):
        ttk.Frame.__init__(self, parent)    # chiamo il costruttore della clase base
        self.parent = parent                # tengo memoria di chè il padre di questa finestra dichiarando la variabile
                                            # parent all'interno del costruttore. Ora parent è accessibile come
                                            # attributo

        self.initUI()                       # chiamata al metodo di inizializzazione dell'interfaccia

    def initUI(self):

        self.pack(side="top", fill="both", expand=True)
        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)

        # crazione del menu del mainframe ------------------------------------------------------------------------------

        menubar = tk.Menu(self.parent)      # dichiaro una variabile menubar dicendo che è figlia del padre di questa finestra
        self.parent.config(menu=menubar)    # imposto il menù che sto creando come quallo dell'applicazione principale

        # creo il primo menu: File
        filemenu = tk.Menu(menubar)
        filemenu.add_command(label="New project...", command=self.onNewProject)
        filemenu.add_command(label="Open project", command=self.onOpenProject)
        filemenu.add_separator()
        filemenu.add_command(label="Exit", command=self.onExit)

        # aggiungo i menu alla barra dei menu
        menubar.add_cascade(label="File", menu=filemenu)

        #label = ttk.Label(self, text="Esempio label", font=LARGE_FONT)
        #label.pack(padx=10, pady=10)

        #button = ttk.Button(self, text="Prova", command=qf)
        #button.pack()

    def onExit(self):
        self.quit()

    def onNewProject(self):
        dialog = AENewProject.AENewProject(self, "prova prova")
        #risultato = dialog.result

    def onOpenProject(self):
        AEUtils.showMessage("Apri progetto", "Open project")


