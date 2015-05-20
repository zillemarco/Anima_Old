__author__ = 'marco'

import tkinter as tk        # importo il modulo Tkinter per gestire l'app ed i widget
import tkinter.ttk as ttk   # importo il modulo Tkinter.ttk per poter utilizzare dei controlli con uno stile visivo migliore

NORMAL_FONT = ("Verdana", 10)

def showMessage(message, title):
    popup = tk.Tk()
    popup.geometry("250x75")
    popup.title(title)

    frame = ttk.Frame(popup)
    frame.pack(side="top", fill="both", expand=True)
    frame.grid_rowconfigure(0, weight=1)
    frame.grid_columnconfigure(0, weight=1)

    label = ttk.Label(frame, text=message, font=NORMAL_FONT)
    label.pack(side="top", fill="x", pady=10, padx=10)

    btnOk = ttk.Button(frame, text="Ok", command=popup.destroy)
    btnOk.pack()

    popup.mainloop()