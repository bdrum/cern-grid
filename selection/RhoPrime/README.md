# UPC triggers

Необходимые данные для анализа задачи ультраперифирических взаимодействий в 2015 году набирались с использованием триггеров CCUP8, CCUP9.

Эти триггеры специально создавались под эту задачу. Об этом сообщается на митингах рабочей группы.

Найдем номера ранов, которые в 2015 году соответствовали этим триггерам.

Для этого зайдем в [логбук Алисы](https://alice-logbook.cern.ch/logbook/date_online.php?p_cont=lc&p_cpn=1&p_cvm=Compact&pcf_crn=245145%2C246994)

Затем перейдем в меню [Runs-->Statistics->Trigger Classes](https://alice-logbook.cern.ch/logbook/date_online.php?p_cont=rtcl&p_rsob=l.run&p_rsob_dir=DESC&p_rspn=1&prsf_rpart=unset):
![](https://pp.userapi.com/c850528/v850528008/164aca/JGYmt6RPcm0.jpg)

Здесь можно искать раны в соответствии со многими параметрами. На данный момент нас интересуют Partition = Physics1 и Trigger Classes CCUP8, CCUP9:

![](https://pp.userapi.com/c849132/v849132008/1c249b/fQFpTHI5p2s.jpg)

Чтобы узнать состав триггера необходимо на странице подробной информации о ране перейти на вкладку trigger info:

![](https://pp.userapi.com/c849424/v849424341/1c5c44/PNBhpYXdVh8.jpg)
 
Информация ниже взята из вкладки Expert View:

CCUP8-B-NOPF-CENTNOTRD 33 DCUP8 CENTNOTRD {NONE} {BCM1} 0 0 0 0 33

CCUP8-A-NOPF-CENTNOTRD 34 DCUP8 CENTNOTRD {NONE} {BCM2} 0 0 0 0 34

CCUP8-C-NOPF-CENTNOTRD 35 DCUP8 CENTNOTRD {NONE} {BCM3} 0 0 0 0 35

CCUP8-E-NOPF-CENTNOTRD 36 DCUP8 CENTNOTRD {NONE} {BCM4} 0 0 0 0 36

CCUP9-B-NOPF-CENTNOTRD 41 DCUP9 CENTNOTRD {NONE} {BCM1} 2085528 0 0 0 41

CCUP9-A-NOPF-CENTNOTRD 42 DCUP9 CENTNOTRD {NONE} {BCM2} 2085528 0 0 0 41

CCUP9-C-NOPF-CENTNOTRD 43 DCUP9 CENTNOTRD {NONE} {BCM3} 2085528 0 0 0 41

CCUP9-E-NOPF-CENTNOTRD 44 DCUP9 CENTNOTRD {NONE} {BCM4} 2085528 0 0 0 41

Распишем что означает каждый из компонентов триггера:

**CCUP8-E-NOPF-CENTNOTRD 36 DCUP8 CENTNOTRD {NONE} {BCM4} 0 0 0 0 36**

> CCUP8 - верхнеуровневое название триггера

> B - направление пучка и маска триггера B - означает физический анализ(нужен мне). А - пучок со стороны A на C ничего нет, если пусто, то изучается фон, E - нет пучков и изучение шумов

> NOPF - no past future? TODO:

> CENTNOTRD - 3 SPD SDD SSD TPC TOF CPV T0 V0 ZDC EMCal AD

> 36 TODO:

>DCUP8 - *0VBA *0VBC *0UBA *0UBC 0STP 0OMU

> CENTNOTRD - 

> {NONE}  TODO:

> {BCM1}  TODO:

> 0(первое число) - это понижающий(DownScale) фактор закодированный во внутренний формат CTP. Напрмер - 1660377 = (0.20827)  
>Для декодирования использовать: [AliTriggerClass::GetDownscaleFactor(Double_t& ds)](https://github.com/alisw/AliRoot/blob/master/STEER/ESD/AliTriggerClass.cxx)

0
0
0
33 TODO:

0STP SPD 0 35 14 26

0OMU TOF 0 61 12 33


Как видно все триггеры состоят из нескольких низкоуровневых треггиеров, каждйы из котороых связан с определенным детектором, в частности:

DCUP8 - 

*0VBA - звезда обозначает вето. на v0 A не должно быть сигнала

*0VBC 

*0UBA - ?TODO:

*0UBC - ?TODO:

0STP - SPD

0OMU - TOF


DCUP9  *0VBA *0VBC *0UBA *0UBC 0STP

Интересное наблюдение, что в CUP8 добавлен еще один тригер на TOF по сравнению с CUP9, что казалось, должно урезать данные, однако данных с CUP8 гораздо больше, это свяазно с DawnScaling, коэфициент которого можно посмотреть на вкладке Classes:
![](https://pp.userapi.com/c858128/v858128341/16a0/nffkVCZdkv4.jpg)

Как видно для CCUP8 масштабирование отсутсвует, в то время как для CCUP9 оно составляет 0.5%, что означает, что лишь такой процент от данных будет записан.



DCUP29  *0VBA *0VBC *0UBA *0UBC 0STG 
DCUP30  *0VBA *0VBC *0UBA *0UBC 0STG 0OM2
DCUP31  *0VBA *0VBC *0UBA *0UBC 0STG 0OMU


0UBA AD 0 56 4 40

0UBC AD 0 57 5 41

0VBA V0 0 52 1 6

0VBA V0 M x 1 6

0STG SPD 0 37 16 26

0OMU TOF 0 61 13 33

0OM2 TOF 0 60 12 31

