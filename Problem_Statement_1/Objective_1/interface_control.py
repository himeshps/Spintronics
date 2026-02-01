import tkinter as tk
import serial
import time
from tkinter import messagebox

PORT = "COM5"   # Change if needed
BAUD = 115200
ser = None

def connect():
    global ser
    try:
        ser = serial.Serial(PORT, BAUD, timeout=1)
        time.sleep(2)
        status.config(text="Connected", fg="green")
    except:
        messagebox.showerror("Error", "Serial connection failed")

def move(direction):
    if not ser:
        messagebox.showwarning("Warning", "Not connected")
        return
    try:
        steps = int(steps_entry.get())
        speed = int(speed_entry.get())
        if direction == "reverse":
            steps = -steps
        ser.write(f"MOVE {steps} {speed}\n".encode())
    except:
        messagebox.showerror("Error", "Invalid input")

root = tk.Tk()
root.title("Stepper Motor Control")

tk.Button(root, text="Connect", command=connect).pack(pady=5)
status = tk.Label(root, text="Disconnected", fg="red")
status.pack()

tk.Label(root, text="Steps").pack()
steps_entry = tk.Entry(root)
steps_entry.pack()

tk.Label(root, text="Speed (steps/sec)").pack()
speed_entry = tk.Entry(root)
speed_entry.pack()

tk.Button(root, text="Forward", command=lambda: move("forward")).pack(pady=5)
tk.Button(root, text="Reverse", command=lambda: move("reverse")).pack(pady=5)

root.mainloop()
