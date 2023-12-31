### Задание 6.2
Написать программу, получающую копии пакетов, предназначенных приложению-серверу из задачи 5.1 (снифер).
Программа может формировать дамп данных, который нужно вручную проанализировать (просмотр бинарных файлов) и убедиться, что это действительно информация от программы-клиента.
Можно дополнить программу так, чтобы она расшифровывала и выводила в файл или на экран полученные сообщения.

Запускается сервер на порту 12321:
![1.server_start.png](/6/6.2/img/1.server_start.png)

Запускается клиент с адресом 127.0.0.1:
![2.client_start.png](/6/6.2/img/2.client_start.png)

Запускается сниффер:
![3.sniffer_start.png](/6/6.2/img/3.sniffer_start.png)

Отправляется сообщение с клиента на сервер:
![4.client_sent.png](/6/6.2/img/4.client_sent.png)

Получение сообщение на сервере:
![5.server_received.png](/6/6.2/img/5.server_received.png)

Сохранение пакетов посредством сниффера:
![6.sniffer_saved.png](/6/6.2/img/6.sniffer_saved.png)

Содержимое 1 пакета:  
![7.packet1.png](/6/6.2/img/7.packet1.png)

Из данных 1 пакета видно, что клиент запустился на 51001 порту (адрес источника). Адрес назначения 12321 - порт сервера. IP совпадает с указанным при запуске клиента. В поле Data Payload находится отправленное сообщение. Точками заменяются все символы, не входящие в диапазон от 32 до 128.

Содержимое 2 пакета:  
![8.packet2.png](/6/6.2/img/8.packet2.png)

Во втором пакете адрес истоника теперь сервер, адрес назначение - клиент, потому что сервер отправляет то же самое сообщение клиенту в ответ.

Если выводить коды символов, то две точки в сообщении - это перевода строки (10) и конца строки (0):
![9.dot_meaning.png](/6/6.2/img/9.dot_meaning.png)
