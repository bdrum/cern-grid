# DataFlow ALICE|CERN

**The description of working with data of ALICE Experiment at CERN**

_[Boris Rumyantsev](mailto:bd.rumyantsev@gmail.com)_

_These notes are result of my job in this direction_

- [DataFlow ALICE|CERN](#dataflow-alicecern)
  - [Data access](#data-access)
    - [GRID](#grid)
    - [DataSignature](#datasignature)
  - [Data analysis](#data-analysis)
    - [Data acquisition](#data-acquisition)
      - [Creation of analysis scripts](#creation-of-analysis-scripts)
      - [Choosing of runs](#choosing-of-runs)

## Data access

Результатом стлокновений является также ран, который имеет порядковый номер (296690, ...). Вся работа с данными организована через распределенную [GRID систему](http://grid.jinr.ru/)

### GRID

Для доступа к системе грид, необходимо иметь актуальный сертификат, который действует в течение одного года. Выданный сертификат подразумевает наличие файлов:

~~~bash
 /# Обрати внимание, что файлы и папка должны иметь соответствующие права и пользователя
[~/.globus]$ ls -la
drwxrwxr-x  2 bdrum bdrum 4096 мая 14 17:05 .
-rwxrwxr-x  1 bdrum bdrum 2472 мая 14 17:05 cert.p12
-rwxrwxr-x  1 bdrum bdrum 1907 мая 14 17:05 usercert.pem
-rwxrwxr-x  1 bdrum bdrum  963 мая 14 17:05 userkey.pem
-rwxrwxr-x  1 bdrum bdrum  820 мая 14 17:05 userreq.mail
~~~

Затем, необходимо иметь клиент для доступа к ГРИД. Такой клиент реализован в aliroot, т.е. необходимо установить [AliPhysics](https://alice-doc.github.io/alice-analysis-tutorial/building/build.html)

Список установленных пакетов

~~~bash
alienv q
~~~

Вход в конкретный пакет
~~~bash
alienv enter AliPhysics/latest
/# alienv enter VO_ALICE@AliPhysics::vAN-20190513_ROOT6-1 # in some special case
~~~

Вход в ГРИД:

~~~bash
alien-token-init $userName
~~~

Доступ дается на сутки, затем для доступа к данным 

~~~bash
aliensh
aliensh:[alice] [..]cd /alice/data/2018/LHC18r/000296690
~~~
Данные находятся в системе [GRID]() в соответствующей директории: 
~~~bash
aliensh:[alice] [23] /alice/data/2018/LHC18r/000296690/ >ls -la
drwxr-xr-x   alidaq   alidaq   0 Dec 04 14:23    .                                 
drwxr-xr-x   alidaq   alidaq   0 Nov 21 13:52    ..                                 
crwxr-xr-x   alidaq   alidaq   - Dec 04 14:23    collection                                 
crwxr-xr-x   alidaq   alidaq   - Dec 04 14:23    collection_10p                                 
crwxr-xr-x   alidaq   alidaq   - Dec 04 14:23    collection_90p                                 
crwxr-xr-x   alidaq   alidaq   - Dec 04 14:23    collection_calib                                 
drwxr-xr-x   alidaq   alidaq   0 Dec 04 14:23    cpass0_pass1                                 
drwxr-xr-x   alidaq   alidaq   0 Dec 04 14:23    cpass1_pass1                                 
-rwxr-xr-x   alidaq   alidaq   - Dec 04 14:23    fullrun.xml                                 
drwxr-xr-x   alidaq   alidaq   0 Mar 13 03:45    muon_calo_pass3                                 
drwxr-xr-x   alidaq   alidaq   0 Jan 09 07:45    pass1                                 
drwxr-xr-x   alidaq   alidaq   0 Dec 04 14:23    raw                                 
drwxr-xr-x   alidaq   alidaq   0 Dec 26 16:56    skim_CALOPLUS 
~~~

Директория raw содержит так называемые "сырые", не обработанные данные. Людей, занимающихся анализом, они редко интересуют.

Такие данные подвергаются процессу реконструкции треков. Реконструкция, может проводится в несколько проходов, результаты таких проходов помещаются в директории passN, где N - номер прохода. В нашем случае был только один проход.

~~~bash
aliensh:[alice] [30] /alice/data/2018/LHC18r/000296690/pass1/ > ls -la
...
drwxr-xr-x   alidaq   alidaq              0 Jan 12 21:38    18000296690039.929                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 13 01:28    18000296690039.930                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 13 00:11    18000296690039.931                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 15 06:13    AOD                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 15 06:14    MergedTrees                                 
-rwxr-xr-x   alidaq   alidaq      340818292 Jan 09 07:55    OCDB.root                                 
-rwxr-xr-x   alidaq   alidaq          14491 Jan 09 07:55    ocdb_log_archive.zip                                 
drwxr-xr-x   alidaq   alidaq              0 Feb 09 00:28    PWGDQ                                 
drwxr-xr-x   alidaq   alidaq              0 May 09 23:24    PWGGA                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 17 18:19    PWGHF                                 
drwxr-xr-x   alidaq   alidaq              0 May 13 01:18    PWGJE                                 
drwxr-xr-x   alidaq   alidaq              0 Feb 09 14:34    PWGLF                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 16 11:09    PWGPP                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 22 11:34    PWGUD                                 
drwxr-xr-x   alidaq   alidaq              0 Mar 09 00:31    PWGZZ                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 15 12:05    QA                                 
-rwxr-xr-x   alidaq   alidaq          50052 Jan 09 07:55    rec.log                                 
-rwxr-xr-x   alidaq   alidaq       16321876 Jan 15 06:17    Stage_1.xml                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 15 06:23    Stage_1                                 
-rwxr-xr-x   alidaq   alidaq         812520 Jan 15 08:46    Stage_2.xml                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 15 08:48    Stage_2                                 
-rwxr-xr-x   alidaq   alidaq          49346 Jan 15 09:12    Stage_3.xml                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 15 09:14    Stage_3                                 
-rwxr-xr-x   alidaq   alidaq           7077 Jan 15 09:46    Stage_5.xml                                 
-rwxr-xr-x   alidaq   alidaq            416 Jan 09 07:55    stderr.ocdb.log                                 
-rwxr-xr-x   alidaq   alidaq          35416 Jan 09 07:55    stdout.ocdb.log                                 
~~~

В директории "проходов" пока нас интересуют только директории [AOD(Analysis object data)](https://twiki.cern.ch/twiki/bin/viewauth/ALICE/AODsets), это данные агрегированные в ОО парадигме. И уже внутри этой директории можно найти сами данные:

~~~bash
aliensh:[alice] [33] /alice/data/2018/LHC18r/000296690/pass1/AOD/ >ls
...
9997
9998
9999
aod_collection.xml
aliensh:[alice] [35] /alice/data/2018/LHC18r/000296690/pass1/AOD/9999/ >ls -la
drwxr-xr-x   alidaq   alidaq              0 Jan 15 07:43    .                                 
drwxr-xr-x   alidaq   alidaq              0 Jan 15 06:13    ..                                 
-rwxr-xr-x   alidaq   alidaq            149 Jan 15 08:01    1091_1091_0_911.stat                                 
-rwxr-xr-x   alidaq   alidaq        1654309 Jan 15 08:01    AliAOD.Muons.root                                 
-rwxr-xr-x   alidaq   alidaq      945827811 Jan 15 08:01    AliAOD.root                                 
-rwxr-xr-x   alidaq   alidaq      224698358 Jan 15 08:01    AliAOD.VertexingHF.root                                 
-rwxr-xr-x   alidaq   alidaq       86957503 Jan 15 08:01    AliAODGammaConversion.root                                 
-rwxr-xr-x   alidaq   alidaq        3662124 Jan 15 08:01    AODQA.root                                 
-rwxr-xr-x   alidaq   alidaq          24103 Jan 15 08:01    AOD_log_archive.zip                                 
-rwxr-xr-x   alidaq   alidaq     1271609856 Jan 15 08:01    aod_archive.zip                                 
-rwxr-xr-x   alidaq   alidaq         756953 Jan 15 08:01    EventStat_temp.root                                 
-rwxr-xr-x   alidaq   alidaq        8051721 Jan 15 08:01    FilterEvents_Trees.root                                 
-rwxr-xr-x   alidaq   alidaq          92534 Jan 15 08:01    stderr.log                                 
-rwxr-xr-x   alidaq   alidaq          90827 Jan 15 08:01    stdout.log            
~~~

После получения данных через [запуск скриптов в грид](), они появляются в домашней директории aliensh. Нужно сделать две операции:

TODO: describe the modern approach:
    alihadd
    alisync
    alien_cp
    alien_rsync.sh

1. Копировать данные в рабочую директорию на lxplus. Это можно сделать таким скриптом:

~~~bash
    x=1; for f in $(find . *.root); do x=$((x+1)); cp $f file:/afs/cern.ch/work/b/brumyant/dev/forMerge/$x.root; done;
    /# для копирования большого числа файлов нужно понимать их уникальный путь, иначе не удастся восстановить какие уже скачал
    for f in $(find . *.root); do x=$(/usr/bin/basename $(/usr/bin/dirname $(/usr/bin/dirname $f))).$(/usr/bin/basename $(/usr/bin/dirname $f)).$(/usr/bin/basename $f); cp $f file:/mnt/d/GoogleDrive/Job/cern/Alice/analysis/data/rho/2015/forMerge/$x; done;
~~~

1. Объединить копированные данные в один root-file:

~~~bash
   /# hadd result.root file1.root file2.root ... fileN.root
    hadd result.root *.root
~~~

hadd - утилита поставляемая вместе с root в $ROOTSYS/bin/hadd.

### DataSignature

Данные находятся на grid-серверах и распределены по директориям в соответствии с такой схемой:

~~~bash
alice
└── data
    └── 2018
        └── LHC18r
            └── 000296690
                ├── pass1
                │   ├── 18000296690039.100
                │   ├── 18000296690039.931
                │   │   └── AliESDs.root
                │   ├── 18000296690039.***
                │   ├── AOD
                │   │   ├── 001
                │   │   ├── ***
                │   │   └── 9999
                │   │       └── AliAOD.root
                │   └── PWGUD
                │       └── UD_PbPb_ESD
                │           └── 243_20191110-1108_child_2
                │               └── 0342
                │                   └── AnalysisResults.root
                └── pass3
                    ├── 18000296690039.100
                    ├── 18000296690039.***
                    ├── 18000296690039.627
                    │   └── AliESDs.root
                    └── 18000296690039.***
~~~

То есть полные пути к файлам будут выглядеть следующим образом:

~~~bash
/alice/data/2018/LHC18r/000296690/pass1/18000296690039.931/AliESDs.root
/alice/data/2018/LHC18r/000296690/pass1/PWGUD/UD_PbPb_ESD/243_20191110-1108_child_2/0342/AnalysisResults.root
~~~

## Data analysis

### Data acquisition

Анализ данных в ЦЕРН можно проводить двумя способами:

1. Набирать данные из системы грид к себе на локальную машину в соответствии со своими идеями и задачами, после чего заниматься их анализом локально. Идея в том, чтобы выгрузить как можно большее количество данных, которые потом можно свободно анализировать на локальной машине не завися от квот на использование grid.
   Как можно больший объем при этом, очевидно, имеет разумный предел. Так, например, для задач UPC файл из 40 ранов по одному периоду может достигать 10ГБ размера. При этом используя современные классы RDataFrame его можно обработать за одну минуту.
2. В некоторых задачах (фемтоскопия) это не подходит, поэтому и сам анализ происходит в гриде.

Несмотря на это, формально, подход строго унифицирован и требует задействование грид системы, а также понимания того как устроены данные как в вопросе их хранения, то есть какие раны нужно выбрать, так и в вопросе из обработки по конкретным триггерам и классам aliroot.

#### Creation of analysis scripts

Задача набора и анализа данных проводится через aliroot с помощью класса:

[AliAnalysisTaskSE](http://alidoc.cern.ch/AliRoot/master/class_ali_analysis_task_s_e.html)

Который в конечном счете наследуется от базового класса TTask.
[Концепция задач(тасков) в документации рут](https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#tasks)

Таким образом реализуется единая схема анализа данных.
Так выглядит директория, которая содержит файлы необходимые для анализа:

~~~bash
.
├── AddTaskUpcRho0.C
├── AliAnalysisTaskUpcRho0.cxx
├── AliAnalysisTaskUpcRho0.h
└── runAnalysis.C
~~~

Рассмотрим файлы в отдельности:

AliAnalysisTaskUpcRho0.h

~~~c++
 #ifndef ALIANALYSISTASKUPCRHO0_H
 #define ALIANALYSISTASKUPCRHO0_H

class TClonesArray;
class TFile;
class TTree;
class TList;
class TH1;
class TList;
class AliPIDResponse;
class AliESDEvent;
class TBits;

 #include "AliAnalysisTaskSE.h"

class AliAnalysisTaskUpcRho0 : public AliAnalysisTaskSE {
public:
  AliAnalysisTaskUpcRho0();
  AliAnalysisTaskUpcRho0(const char *name, Bool_t);
  virtual ~AliAnalysisTaskUpcRho0();

  virtual void Init();
  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *){};

  void SetIsMC(Bool_t _isMC) { isMC = _isMC; }
  void SetEfficiencyFileName(TString _fEfficiencyFileName) {
    fEfficiencyFileName = _fEfficiencyFileName;
    isUsingEffi = kTRUE;
  }
  void SetTrigger(TString _fTriggerName) { fTriggerName = _fTriggerName; }

private:
  Bool_t Is0STPfired(Int_t *, Int_t *);
  Bool_t IsTriggered(AliESDEvent *);

  Int_t TPCsignal_T[2];
  Float_t TrackP_T[2];

  Float_t TrackEta_T[2];
  Float_t TrackPhi_T[2];
  Float_t TrackPx_T[2];
  Float_t TrackPy_T[2];
  Float_t TrackPz_T[2];


  AliPIDResponse *fPIDResponse;
  TClonesArray *GenPart_T;

  TList *fListHist;
  TH1I *fHistTriggersPerRun;


  AliAnalysisTaskUpcRho0(const AliAnalysisTaskUpcRho0 &); // not implemented
  AliAnalysisTaskUpcRho0 &
  operator=(const AliAnalysisTaskUpcRho0 &); // not implemented

  ClassDef(AliAnalysisTaskUpcRho0, 7);
};

 #endif
~~~

Как видно из заголовочного файла, аналитик создает свой класс(AliAnalysisTaskUpcRho0) который наследуется от AliAnalysisTaskSE

Самые главные методы это:

* **AliAnalysisTaskUpcRho0()**; - конструктор по умолчанию
* **AliAnalysisTaskUpcRho0(const char *name, Bool_t);** - конструктор содержащий два параметра имя задачи и логическую переменную указывающую на монте-карло ли это или нет. Вообще может быть определен любой конструктор.
* **virtual ~AliAnalysisTaskUpcRho0();** - виртуальный деструктор должен освобождать набираемые ресурсы. Например fListHist
* **virtual void Init();** - метод который задает начальные значения тем полям, для которых это необходимо. Вызывается конструктором.
* **virtual void UserCreateOutputObjects();** - определяет объекты, которые будут на выходе. То есть у полей типа дерево задаются листья и какие поля туда войдут, объявляются гистограммы и указывается какие поля туда войдут и так далее.
* **virtual void UserExec(Option_t *option);** - функция которая будет выполнятся для каждого события. Здесь ведется отсев данных и расчеты. Здесь можно получить доступ к трекам и треклетам.
* **_SelectCollisionCandidates(UInt_t trigger_mask)_** - не является обязательной, но вызывается до userexec, так что UserExec будет вызвана только для тех событий, что прошли проверку.
* **virtual void Terminate(Option_t *){};** - запускается в самом конце анализа. часто оставляется пустой. хорошо подходит для финальных проверок.

Прежде чем приступить к анализу данных необходимо их выбрать.

#### Choosing of runs

Прежде чем анализировать данные, нужно понимать какие данные нужны и откуда их взять. Все данные хранятся, как описано выше, на серверах ЦЕРНа.

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

> B - направление пучка

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

В коде каждому триггеру выставляется [бит](https://github.com/alisw/AliRoot/blob/master/STEER/STEERBase/AliVEvent.h).

~~~cpp
enum EOfflineTriggerTypes { 
    kMB                = BIT( 0), // Minimum bias trigger in PbPb 2010-11
    kINT1              = BIT( 0), // V0A | V0C | SPD minimum bias trigger
    kINT7              = BIT( 1), // V0AND minimum bias trigger
    kMUON              = BIT( 2), // Single muon trigger in pp2010-11, INT1 suite
    kHighMult          = BIT( 3), // High-multiplicity SPD trigger
    kHighMultSPD       = BIT( 3), // High-multiplicity SPD trigger
    kEMC1              = BIT( 4), // EMCAL trigger in pp2011, INT1 suite
    kCINT5             = BIT( 5), // V0OR minimum bias trigger
    kINT5              = BIT( 5), // V0OR minimum bias trigger
    kCMUS5             = BIT( 6), // Single muon trigger, INT5 suite
    kMUSPB             = BIT( 6), // Single muon trigger in PbPb 2011
    kINT7inMUON        = BIT( 6), // INT7 in MUON or MUFAST cluster
    kMuonSingleHighPt7 = BIT( 7), // Single muon high-pt, INT7 suite
    kMUSH7             = BIT( 7), // Single muon high-pt, INT7 suite
    kMUSHPB            = BIT( 7), // Single muon high-pt in PbPb 2011
    kMuonLikeLowPt7    = BIT( 8), // Like-sign dimuon low-pt, INT7 suite
    kMUL7              = BIT( 8), // Like-sign dimuon low-pt, INT7 suite
    kMuonLikePB        = BIT( 8), // Like-sign dimuon low-pt in PbPb 2011
    kMuonUnlikeLowPt7  = BIT( 9), // Unlike-sign dimuon low-pt, INT7 suite
    kMUU7              = BIT( 9), // Unlike-sign dimuon low-pt, INT7 suite
    kMuonUnlikePB      = BIT( 9), // Unlike-sign dimuon low-pt in PbPb 2011
    kEMC7              = BIT(10), // EMCAL/DCAL L0 trigger, INT7 suite
    kEMC8              = BIT(10), // EMCAL/DCAL L0 trigger, INT8 suite
    kMUS7              = BIT(11), // Single muon low-pt, INT7 suite
    kMuonSingleLowPt7  = BIT(11), // Single muon low-pt, INT7 suite
    kPHI1              = BIT(12), // PHOS L0 trigger in pp2011, INT1 suite
    kPHI7              = BIT(13), // PHOS trigger, INT7 suite
    kPHI8              = BIT(13), // PHOS trigger, INT8 suite
    kPHOSPb            = BIT(13), // PHOS trigger in PbPb 2011
    kEMCEJE            = BIT(14), // EMCAL/DCAL L1 jet trigger
    kEMCEGA            = BIT(15), // EMCAL/DCAL L1 gamma trigger
    kHighMultV0        = BIT(16), // High-multiplicity V0 trigger
    kCentral           = BIT(16), // Central trigger in PbPb 2011
    kSemiCentral       = BIT(17), // Semicentral trigger in PbPb 2011
    kDG                = BIT(18), // Double gap diffractive
    kDG5               = BIT(18), // Double gap diffractive
    kZED               = BIT(19), // ZDC electromagnetic dissociation
    kSPI7              = BIT(20), // Power interaction trigger
    kSPI               = BIT(20), // Power interaction trigger
    kINT8              = BIT(21), // 0TVX trigger
    kMuonSingleLowPt8  = BIT(22), // Single muon low-pt, INT8 suite
    kMuonSingleHighPt8 = BIT(23), // Single muon high-pt, INT8 suite
    kMuonLikeLowPt8    = BIT(24), // Like-sign dimuon low-pt, INT8 suite
    kMuonUnlikeLowPt8  = BIT(25), // Unlike-sign dimuon low-pt, INT8 suite
    kMuonUnlikeLowPt0  = BIT(26), // Unlike-sign dimuon low-pt, no additional L0 requirement
    kUserDefined       = BIT(27), // Set when custom trigger classes are set in AliPhysicsSelection
    kTRD               = BIT(28), // TRD trigger
    kMuonCalo          = BIT(29), // Muon-calo triggers
    kCaloOnly          = BIT(29), // MB, EMCAL and PHOS triggers in CALO or CALOFAST cluster
    // Bits 30 and above are reserved for FLAGS
    kFastOnly          = BIT(30), // The fast cluster fired. This bit is set in to addition another trigger bit, e.g. kMB
    kAny               = 0xffffffff, // to accept any defined trigger
    kAnyINT            = kMB | kINT7 | kINT5 | kINT8 | kSPI7 // to accept any interaction (aka minimum bias) trigger
};
~~~

Выбор ранов осуществлялся такими фильтрами
![](https://sun9-7.userapi.com/c854028/v854028195/18b98d/2zDtgvdzTbY.jpg)


[^1]: [Презентация](file://D:\GoogleDrive\Job\cern\Alice\study\AliceDataFlow.pdf)
