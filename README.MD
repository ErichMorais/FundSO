**************************TRABALHO DE FUNDAMENTOS DOS SISTEMAS OPERACIONAIS (2019/2)*****************************

*******************************Douglas Santos e Erich Morais***********************************

Passos para execução do programa:

1 - sudo tail -f /var/log/syslog

2 - sudo mknod /dev/damas c 60 0

3 - sudo chmod 666 /dev/damas

4 - Ir para a pasta de aplicação
  4.1 - make

5 - sudo insmod driver.ko

6 - (Apenas em caso de alteração no driver)
  6.1 - make
  6.2 - sudo rmmod driver.ko
  6.3 - sudo insmod driver.ko


7 - **********************Referências*********************
  7.1 - http://www.tldp.org/LDP/lkmpg/2.4/html/x856.html
