#!/usr/bin/env python3
import serial
import tkinter as tk


if __name__ == '__main__':

    update_water_level = False
    depth = 10.0
    port_busy = False

    # GUI
    ws = tk.Tk()
    ws.title('Tanque')
    ws.config(bg='#0EBAD1')
    ws.geometry('400x300')

    label_water_level = tk.Label(ws, bg='#0EBAD1', text='Cargando...', font=18)
    label_water_level.pack(expand=True)

    label_depth = tk.Label(ws, bg='#0EBAD1', font=18)
    label_depth.pack(expand=True)

    def get_levl_clicked():
        global port_busy
        if not port_busy:
            port_busy = True
            ser.write(b"get levl\n")
            label_water_level.config(text="Carga: " + ser.readline().decode('utf-8').rstrip())
            port_busy = False

    def toggle_get_water_level():
        global update_water_level
        if update_water_level:
            update_water_level = False
            btn_toggle_get_water_level['text'] = 'Lectura de Carga OFF'
        else:
            update_water_level = True
            btn_toggle_get_water_level['text'] = 'Lectura de Carga ON'

    def toggle_autofill():
        global port_busy
        if not port_busy:
            port_busy = True
            ser.write(b"get atfl\n")
            val = ser.readline().decode('utf-8').rstrip()
            if "1" in val:
                ser.write(b"set atfl 0")
                btn_toggle_autofill['text'] = "Llenado Automatico OFF"
            elif "0" in val:
                ser.write(b"set atfl 1")
                btn_toggle_autofill['text'] = "Llenado Automatico ON"
            port_busy = False

    def set_depth():
        global depth
        text_input = entry_depth.get()
        try:
            new_depth = float(text_input)
            if new_depth >= 3.0:
                ser.write(("set dept " + text_input.strip()).encode('utf-8'))
                depth_response = ser.readline().decode('utf-8').rstrip()
                label_depth.config(text="Profundidad: " + depth_response + "cm")
                depth = new_depth
        except:
            entry_depth.delete(0, 'end')


    btn_toggle_get_water_level = tk.Button(ws, text='Lectura de Carga OFF', padx=10, pady=5,
                                           command=toggle_get_water_level)
    btn_toggle_get_water_level.pack(expand=True)

    btn_toggle_autofill = tk.Button(ws, text='Llenado Automatico', padx=10, pady=5, command=toggle_autofill)
    btn_toggle_autofill.pack(expand=True)

    entry_depth = tk.Entry(ws)
    entry_depth.pack(expand=True)

    btn_set_depth = tk.Button(ws, text='Enviar Profundidad', command=set_depth)
    btn_set_depth.pack(expand=True)

    def message():
        if update_water_level:
            get_levl_clicked()
        ws.after(5000, message)

    def setup():
        global depth
        ser.write(b"get atfl\n")
        val = ser.readline().decode('utf-8').rstrip()
        if "1" in val:
            btn_toggle_autofill['text'] = "Llenado automatico " + "ON"
        else:
            btn_toggle_autofill['text'] = "Llenado automatico " + "OFF"
        ser.write(b"get dept\n")
        depth_response = ser.readline().decode('utf-8').rstrip()
        label_depth.config(text="Profundidad: " + depth_response + "cm")
        depth = float(depth_response)
        get_levl_clicked()
        ws.after(5000, message)

    # Serial
    # __________________________________
    connected = False
    ser = None
    port = 'com3'
    for i in range(0, 20):
        try:
            port = 'com' + str(i)
            ser = serial.Serial(port, 9600, timeout=10)  # ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
            ser.reset_input_buffer()
        except:
            continue
        else:
            print("Connected to port", port)
            connected = True
            break

    if not connected:
        print("Could not connect serial")
    else:
        # Activate GUI
        ws.after(2000, setup)
        ws.mainloop()
        ser.close()
