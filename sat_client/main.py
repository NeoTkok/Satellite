import socket
import time
import threading
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import tkinter as tk

client_ID = 404
connected = 0
# container for animation (JUST FOR 1 SAT!!!)
coords_container = [[np.nan, np.nan, np.nan]]


def animate(i):
    data = np.array(coords_container)
    ax.clear()
    ax.set_xlim(-1e7, 1e7)
    ax.set_ylim(-1e7, 1e7)
    ax.set_zlim(-1e7, 1e7)
    ax.scatter(data[:, 0], data[:, 1], data[:, 2], c="green")
    ax.scatter(data[-1, 0], data[-1, 1], data[-1, 2], c="black", s=20, marker="H")
    rx, ry, rz = 6400000, 6400000, 6400000
    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(0, np.pi, 100)
    x = rx * np.outer(np.cos(u), np.sin(v))
    y = ry * np.outer(np.sin(u), np.sin(v))
    z = rz * np.outer(np.ones_like(u), np.cos(v))

    ax.plot_wireframe(x, y, z, alpha=0.1)

    max_radius = max(rx, ry, rz)
    for axis in 'xyz':
        getattr(ax, 'set_{}lim'.format(axis))((-max_radius, max_radius))


# pushes new coords into container for animation
def push_coords(r):
    if len(coords_container) < 30:
        coords_container.append(r)
    else:
        coords_container.pop(0)
        coords_container.append(r)
    # print(coords_container)


def send_tmp_func(msg):
    length = chr(len(msg))
    sock.send(length.encode('utf-8'))
    time.sleep(0.01)
    sock.send(msg.encode('utf-8'))
    time.sleep(0.01)


def sat_onl(refresh_type):
    if refresh_type == 0:
        send_tmp_func("refresh")
        for widget in button_frame.winfo_children():
            widget.destroy()
        btn2 = tk.Button(button_frame,
                         text='Обновить',
                         command=lambda: sat_onl(1),
                         bg='#C9CDF5',
                         activebackground='#BEC4F5',
                         font=('Arial', 10)
                         )
        btn2.pack()
    if refresh_type == 1:
        send_tmp_func("back")
        send_tmp_func("refresh")
    if refresh_type == 2:
        send_tmp_func("back")
        for widget in id_frame.winfo_children():
            widget.destroy()
        for widget in button_frame.winfo_children():
            widget.destroy()
        btn1 = tk.Button(button_frame,
                         text='Показать доступные спутники',
                         command=lambda: sat_onl(0),
                         bg='#C9CDF5',
                         activebackground='#BEC4F5',
                         borderwidth='1',
                         font=('Arial', 15)
                         )
        btn1.pack()


def entry_func(msg_str):
    if msg_str.get():
        send_tmp_func(msg_str.get())
    else:
        tk.messagebox.showerror(title='Ошибка', message='Пустое окно отправки. Повторите попытку.')


def broadcast_func(br_type):
    global broadcast_coord_btn
    if br_type == 0:
        send_tmp_func("broadcast coords")
        broadcast_coord_btn['text'] = 'Остановить трансляцию координат'
        broadcast_coord_btn['command'] = lambda: broadcast_func(1)
    if br_type == 1:
        send_tmp_func("stop broadcast")
        broadcast_coord_btn['text'] = 'Транслировать координаты'
        broadcast_coord_btn['command'] = lambda: broadcast_func(0)



def analyse_data(data):
    # push is only added, if get coords cont function was called (see sat code)
    # better to add push on server
    global get_coord_btn
    global broadcast_coord_btn
    if data[0:4] == "push":
        r_str = data[5:].split(" ")
        r = [float(r_str[0]), float(r_str[1]), float(r_str[2])]
        push_coords(r)
    if data[0:2] == "ID":
        r_str = data[4:-1].split("\n")
        # Очищаем фрейм
        for widget in id_frame.winfo_children():
            widget.destroy()
        # Начинаем вывод
        info_sats = tk.Label(id_frame, text='Спутники онлайн:', bg='#C9CDF5', font=('Arial', 10)).pack()

        id_labels = []
        for r in r_str:
            print('---------------')
            print(r)
            id_labels.append(tk.Label(id_frame,
                                      text=r,
                                      font=('Arial', 15)
                                      )
                             )
        for i in range(len(id_labels)):
            id_labels[i].pack(padx=10, pady=10)

        btn3 = tk.Button(id_frame,
                         text='Выйти',
                         command=lambda: sat_onl(2),
                         bg='#9698C6',
                         font=('Arial', 10)
                         )
        btn3.pack()

        id_str = tk.StringVar()
        id_entry = tk.Entry(id_frame, bg='#C9CDF5', textvariable=id_str).pack()
        id_entry_button = tk.Button(
            id_frame,
            text='Ввести ID',
            bg='#C9CDF5',
            command=lambda: entry_func(id_str)
        )
        id_entry_button.pack()

    data_str = data.split(" ")
    if data_str[0] == "You" and data_str[1] == "have" and data_str[2] == "successfully":
        sat_id = data_str[-1]
        for widget in id_frame.winfo_children():
            widget.destroy()
        for widget in button_frame.winfo_children():
            widget.destroy()
        btn1 = tk.Button(button_frame,
                         text='Показать доступные спутники',
                         command=lambda: sat_onl(0),
                         bg='#C9CDF5',
                         activebackground='#BEC4F5',
                         borderwidth='1',
                         font=('Arial', 15)
                         )
        btn1.pack()

        id_label = tk.Label(
            id_frame,
            text=f'Подключен к спутнику: {sat_id}',
            bg='#C9CDF5',
            font=('Arial', 15),
            pady=10
        )
        id_label.pack()

        get_coord_btn = tk.Button(
            id_frame,
            text=f'Получить координаты:',
            command=lambda: send_tmp_func("get coords"),
            font=('Arial', 10)
        )
        get_coord_btn.pack(pady=10)

        broadcast_coord_btn = tk.Button(
            id_frame,
            text='Транслировать координаты',
            command=lambda: broadcast_func(0),
            font=('Arial', 10)
        )
        broadcast_coord_btn.pack(pady=10)

        graphon_button = tk.Button(id_frame,
                                   text='Показать график',
                                   command=graphon_func
                                   )
        graphon_button.pack(pady=10)

    if data_str[0] == "coord":
        get_coord_btn['text'] = f'Получить координаты: {data_str[1]} {data_str[2]} {data_str[3]}'


# here we listen to server responds
def client_handler():
    global connected
    while connected:
        data_len = sock.recv(1)  # try 1 and 2 else
        lengh_rcv = ord(data_len)
        time.sleep(0.1)

        data = sock.recv(lengh_rcv + 3)
        data_str = data.decode('utf-8')[3:]
        if data_str[0:4] != "push" and data_str[0:2] != "ID" and data_str != "You left successfully" and data_str[
                                                                                                         0:5] != "coord":
            print(data_str)
            tk.messagebox.showinfo(title='MSG From Satellite', message=data_str)
        analyse_data(data_str)
        # first 3 bite's are not message
        time.sleep(0.1)


# here we can type and send requests on server
def client_input():
    global connected
    if connected:
        msg = "help"
    while connected:
        msg = str(input())
        length = chr(len(msg))
        if msg == "disconnect":
            sock.send(length.encode('utf-8'))
            time.sleep(0.01)
            sock.send(msg.encode('utf-8'))
            time.sleep(0.01)
            connected = 0
        else:
            sock.send(length.encode('utf-8'))
            time.sleep(0.01)
            sock.send(msg.encode('utf-8'))
            time.sleep(0.01)
    print("Bye!")


# sock = socket.socket()
# sock.connect(('192.168.58.225', 1111))
# connected = 1
# sock.send("1\0".encode('utf-8'))
# sock.send(f"{client_ID}\0".encode('utf-8'))
#
# x = threading.Thread(target=client_handler, args=())
# print("Client connected!")
# x.start()
#
# y = threading.Thread(target=client_input, args=())
# y.start()

# print("Available commands:\nrefresh - Get information about Satellites online, connect to them\ndisconnect - "
#       "Disconnect from server\nget coords - Get Satellite coordinates\nbroadcast coords - Broadcast Satellite "
#       "coordinates (with drawing)\nstop broadcast - Stop broadcasting coordinates\nadd impulse - Transmit impulse to "
#       "Satellite\n")
# print("Type 'refresh' to see available satellites:")

# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
# ani = animation.FuncAnimation(fig, animate, interval=200)
# plt.show()


# socket.close()


# GUI

window = tk.Tk()


def graphon_func():
    global fig
    global ax
    global ani

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ani = animation.FuncAnimation(fig, animate, interval=200)
    plt.show()


window.title('Satellite Handler')
window.geometry('600x600')
window['bg'] = '#C9CDF5'

photo = tk.PhotoImage(file='fakt.png')
window.iconphoto(False, photo)

window.resizable(width=False, height=False)

# canvas = tk.Canvas(window, height=600, width=600)
# canvas.pack()

frame = tk.Frame(window, bg='#C9CDF5')
frame.place(relx=0, rely=0, relwidth=1, relheight=0.05)

connection_frame = tk.Frame(window, bg='#C9CDF5')
connection_frame.place(relx=0, rely=0.05, relwidth=1, relheight=0.13)

button_frame = tk.Frame(window, bg='#C9CDF5')
button_frame.place(relx=0, rely=0.18, relwidth=1, relheight=0.10)

id_frame = tk.Frame(window, bg='#C9CDF5')
id_frame.place(relx=0, rely=0.28, relwidth=1, relheight=0.72)

# Title Frame
title = tk.Label(frame,
                 text='Welcome to Satellite Handler GUI!',
                 bg='#C9CDF5',
                 font=('Arial', 15, 'bold')
                 )
title.pack()


# Connection Frame

def connection_func():
    global connected
    global sock

    ip_adress = entry_message.get()
    sock = socket.socket()
    sock.connect((ip_adress, 1111))
    connected = 1
    sock.send("1\0".encode('utf-8'))
    sock.send(f"{client_ID}\0".encode('utf-8'))

    x = threading.Thread(target=client_handler, args=())
    print("Client connected!")
    x.start()

    y = threading.Thread(target=client_input, args=())
    y.start()
    if (connected):
        tk.messagebox.showinfo(title='Подключено!', message=f'Подключен к адресу {ip_adress}')
        connection_button.config(state=tk.DISABLED)


entry_message = tk.StringVar()
connection_entry = tk.Entry(connection_frame, bg='#C9CDF5', textvariable=entry_message).pack()
connection_button = tk.Button(connection_frame,
                              text='Подключиться',
                              command=connection_func,
                              bg='#C9CDF5',
                              activebackground='#BEC4F5',
                              font=('Arial', 10)
                              )
connection_button.pack()

# handler frame


btn1 = tk.Button(button_frame,
                 text='Показать доступные спутники',
                 command=lambda: sat_onl(0),
                 bg='#C9CDF5',
                 activebackground='#BEC4F5',
                 borderwidth='1',
                 font=('Arial', 15)
                 )
btn1.pack()


def on_close():
    global connected
    if connected:
        send_tmp_func("disconnect")
        connected = 0
    window.destroy()


window.protocol("WM_DELETE_WINDOW", on_close)
window.mainloop()
