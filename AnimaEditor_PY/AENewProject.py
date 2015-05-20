__author__ = 'marco'

import tkinter as tk        # importo il modulo tkinter per gestire l'app ed i widget
import tkinter.ttk as ttk   # importo il modulo tkinter.ttk per poter utilizzare dei controlli con uno stile visivo migliore
import tkinter.ttk.simpledialog as simpledialog
import AEUtils

class AENewProject(simpledialog.Dialog):
    def __init__(self, parent, title):
        simpledialog.Dialog.__init__(self, parent, title)
        self.edtProjectName
        self.edtProjectPath

    def body(self, master):
        ttk.Label(master, text="Project name:", font=AEUtils.NORMAL_FONT).grid(row=0)
        ttk.Label(master, text="Project path:", font=AEUtils.NORMAL_FONT).grid(row=1)

        self.edtProjectName = ttk.Entry(master)
        self.edtProjectPath = ttk.Entry(master)

        self.edtProjectName.grid(row=0, column=1, columnspan=2)
        self.edtProjectPath.grid(row=1, column=1)

        return self.edtProjectName

    def apply(self):
        projectName = self.edtProjectName.get()
        projectPath = self.edtProjectPath.get()

        print(projectName + projectPath)