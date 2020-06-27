# Data analysis in ALICE|CERN

**The description of working with data in ALICE Experiment at CERN**

_[Boris Rumyantsev](mailto:bd.rumyantsev@gmail.com)_

_These notes are result of my work in this direction_

- [Data analysis in ALICE|CERN](#data-analysis-in-alicecern)
  - [Data access](#data-access)
    - [GRID](#grid)
      - [Token](#token)
      - [AliEn](#alien)
      - [Data structure](#data-structure)
  - [Data analysis](#data-analysis)
    - [Creation of analysis scripts](#creation-of-analysis-scripts)
    - [Choosing of runs](#choosing-of-runs)
      - [Physics](#physics)
      - [Triggers](#triggers)
    - [Running analysis](#running-analysis)
      - [Locally](#locally)
      - [Grid test](#grid-test)
      - [Grid full](#grid-full)
      - [LEGO trains](#lego-trains)
    - [Receiving results](#receiving-results)

## Data access

Результатом стлокновений является также ран, который имеет порядковый номер (296690, ...). Вся работа с данными организована через распределенную [GRID систему](http://grid.jinr.ru/)

### GRID

#### Token

Для доступа к системе грид, необходимо иметь актуальный сертификат, который действует в течение одного года.

Инструкция для получения есть здесь http://ca.grid.kiae.ru/RDIG/

Порядок такой:

* для этого нужно следовать инструкции на сайте:
  * запустить скрипт по генерации ключей
  * заполнить анкету 
    * Common Name обязательно Name LastName (nick нельзя)
* Отправить  Громовой Наталье Ивановне скан на почту 
* Дождаться ответа в виде ключа
* Ключ положить в папку .globus, вот так должно выглядеть ее содержимое

~~~bash
 /# Обрати внимание, что файлы и папка должны иметь соответствующие права и пользователя
[~/.globus]$ ls -la
drwxrwxr-x  2 bdrum bdrum 4096 мая 14 17:05 .
-rwxrwxr-x  1 bdrum bdrum 2472 мая 14 17:05 cert.p12
-rwxrwxr-x  1 bdrum bdrum 1907 мая 14 17:05 usercert.pem
-rwxrwxr-x  1 bdrum bdrum  963 мая 14 17:05 userkey.pem
-rwxrwxr-x  1 bdrum bdrum  820 мая 14 17:05 userreq.mail
~~~

где usercert.pem - файл с ключом из письма

* конвертировать сертификат в формат p12 для доступа к грид сервисам из бразуера 

~~~bash
.globus# openssl pkcs12 -export -in cert.pem -inkey userkey.20181106-090218.pem -name "bdrumRDIGCert" -out cert.p12
Enter pass phrase for userkey.20181106-090218.pem:
Enter Export Password:
Verifying - Enter Export Password:
~~~

Каждый год сертификат необходимо продлять.

Затем, необходимо иметь клиент для доступа к ГРИД. 

#### AliEn

Alien - клиент для доступа в грид. Его можно установить отдельно, но лучше всего устанавливать сразу [AliPhysics](https://alice-doc.github.io/alice-analysis-tutorial/building/build.html)

Список установленных пакетов

~~~bash
alienv q
~~~

Вход в конкретный пакет
~~~bash
alienv enter AliPhysics/latest
/# alienv enter VO_ALICE@AliPhysics::vAN-20190513_ROOT6-1 # in some special case
~~~

> **Как я понимаю теперь используется какое-то кэширование входа. Так при попытке выполнить команду, в случае, если отсутствует активный токен, она будет автоматически восстановлен. Таким образом описание ниже более не актуально.**

Вход в ГРИД:

~~~bash
alien-token-init $userName
~~~

Доступ дается на сутки, затем для доступа к данным необходимо зайти в специальный shell - aliensh: 

~~~bash
aliensh
aliensh:[alice] [..]cd /alice/data/2018/LHC18r/000296690
~~~

#### Data structure

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

Каждая из директорий содержит множество вспомогателньых данных, а также данных необходимых для работ специальных групп, например контроль качества данных и так далее: 
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

Директория raw содержит так называемые "сырые", не обработанные, данные. Людей, занимающихся анализом, они редко интересуют.

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

В директории "проходов" пока нас интересуют только директории [AOD(Analysis object data)](https://twiki.cern.ch/twiki/bin/viewauth/ALICE/AODsets), это данные агрегированные в ОО парадигме. И уже внутри этой директории можно найти интересующие рут файлы AliAOD.root:

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


## Data analysis

Анализ данных в ЦЕРН можно проводить двумя способами:

1. Набирать данные из системы грид к себе на локальную машину в соответствии со своими идеями и задачами, после чего заниматься их анализом локально. Идея в том, чтобы выгрузить как можно большее количество данных, которые потом можно свободно анализировать на локальной машине не завися от квот на использование grid.
   Как можно больший объем при этом, очевидно, имеет разумный предел. Так, например, для задач UPC файл из 40 ранов по одному периоду может достигать 10ГБ размера. При этом используя современные классы RDataFrame его можно обработать за одну минуту.
2. В некоторых задачах (фемтоскопия) это не подходит, поэтому и сам анализ происходит в гриде.

Несмотря на это, формально, подход строго унифицирован и требует задействование грид системы, а также понимания того как устроены данные как в вопросе их хранения, то есть какие раны нужно выбрать, так и в вопросе из обработки по конкретным триггерам и классам aliroot.

### Creation of analysis scripts

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

TODO: remove redundant variables

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

### Choosing of runs

Обработка данных, очевидно, ведется в соответствии с тем как они храняться. Так опуская очевидные уровни такие как год и период мы переходим к списку ранов, необходимые для передачи в скрипт анализа.

#### Physics

Самое первое разделение может быть произведено по типу пучка:

* p-p - proton - proton
* Pb-Pb - led led 
* Xe-Xe - xenon - xenon
* p-Pb - proton - led

В зависимости от этого типа удобно рассматривать разные физические явления.

#### Triggers

При наборе данных активно используется триггирование, которое несет двойную цель:

1. Ограничить объем данных:
   Пучки сталкиваются каждые 25нс, т.о. частоту столкновений можно записать как 40MHz (Run1 pp?). Для столкновений p-Pb - 200kHz, Pb-Pb: 2kHz. Частота выбирается с учетом возможнотей детектора, так с детекторов необходимо считать информацию, записать ее и затем подготовить детекторы к новому столкновению. ?Для ALICE pp(25ns) не используется.?
  Это очень много событий, мы не можем записать их все по двум причинам:

   * **Ограничение на время считывания** - Максимальное время считывания у детекторов 1024 мкс на событие, такой детектор может записывать не более ~1000 событий в секунду.  
    ![](https://pp.userapi.com/c845524/v845524302/2014bd/QDOoDWtDR7I.jpg)
   * **Ограниченный объем хранилищ данных**
    Например,время считывания TPC = 500 микросекунд, а объем записываемых данных 25740kB, значит каждую секунду TPC может записывать:
$$\frac{1}{500}*10^6*\frac{25740}{1024*1024}=4.9GB$$
  Даже несмотря на эти ограничения, если начать считывать события в случайный момент времени, оно окажется пустым с вероятностью > 99%.
  Поэтому, нужны  (minimum bias triggers) "пороговые" триггеры, которые позволяют отследить не нулевую активность на детекторах в "онлайне".
  Однако существуют редкие события, которые никогда не наберут статистику с "пороговыми" триггерами, поэтому нужны специальные триггеры редких событий, например большая накопленная энергия на EMCAL, два мюона в мюонном плече и т.д.)

2. Обеспечить работу множества научных групп:
  На эксперименте работает большое число научных групп, каждую из которых интересует разная физика, таким образом исходный канал 2kHz необходимо поделить между ними. Это также происходит путем триггеров.
  Например на группу UPC выделяют 30-40Hz. При этом частота триггера может быть выше, чем выделенные 30-40Hz, в таких случаях применяют downscaling, то есть события пропускают так, что бы заполнить выделенный канал. Вес даунскейлинга можно посмотреть в логбуке:
  ![2](https://pp.userapi.com/c844320/v844320302/1fd09e/Q6Kzwc4uhC8.jpg)
  Так, например, коэф. 0.32% означает, что каждое 300 событие пропускалось. 

TODO: При этом число 22 865  указано из набранных, т.е. общее число событий было  6 859 500? То есть что такое L2a?

Детекторы участвующие в триггировании:

![](https://pp.userapi.com/c850736/v850736302/11ca91/vPEOeSdje-k.jpg)

Для каждого рана мы можем посмотреть список триггеров:
![](https://pp.userapi.com/c851524/v851524150/12744e/sTeJFcwO1iE.jpg)

> **Стоит отметить, что в 2021 году в Run3 триггеров не будет и будут записываться все события**

В aliroot каждому триггеру выставляется соответствующий [бит](https://github.com/alisw/AliRoot/blob/master/STEER/STEERBase/AliVEvent.h).

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

Пример фильтров при выборе ранов:

![3](https://sun9-7.userapi.com/c854028/v854028195/18b98d/2zDtgvdzTbY.jpg)


### Running analysis
TODO: add description

#### Locally

#### Grid test

#### Grid full

#### LEGO trains


### Receiving results

После завершения работы скрипта в гриде. Данные появятся в домашней директории aliensh в той иерархии, которая была задана в скрипте. Кстати на место в aliensh существуют квоты, заполнение своей квоты можно узнать так:

![1](https://sun9-69.userapi.com/c857120/v857120643/16ef2f/FD6W0wwjXw8.jpg)

TODO: можно ли сразу сохранять данные на локальную машину?

Для того, чтобы копировать данные на локальную машину необходимо воспользоваться одной из команд

* alisync - синхронизирует выбранные директории локально и удаленно. Работает параллельно.
* alien_cp - позволяет копировать файлы с использованием регулярных выражений.
* TODO: alien_rsync.sh

Раньше копирование выполнялось с помощью таких скриптов:

~~~bash
    x=1; for f in $(find . *.root); do x=$((x+1)); cp $f file:/afs/cern.ch/work/b/brumyant/dev/forMerge/$x.root; done;
    /# для копирования большого числа файлов нужно понимать их уникальный путь, 
    /# иначе не удастся восстановить какие уже скачал
    /# поэтому лучше использовать скрипт, который сохраняет номера ранов и номер внутренней директории
    for f in $(find . *.root); do x=$(/usr/bin/basename $(/usr/bin/dirname $(/usr/bin/dirname $f))).$(/usr/bin/basename $(/usr/bin/dirname $f)).$(/usr/bin/basename $f); cp $f file:/mnt/d/GoogleDrive/Job/cern/Alice/analysis/data/rho/2015/forMerge/$x; done;
~~~

После обработки файлов может быть огромное количество, например 20 000. Их удобно объединить в один файл. Это можно сделать с помощью утилиты поставляемой вместе с root - ROOTSYS/bin/hadd.
При этом, как я понимаю, лучше пользоваться alihadd, так как эта команда понимает специализированные классы aliroot.

~~~bash
   /# hadd result.root file1.root file2.root ... fileN.root
    hadd result.root *.root

    /# для файлов aliroot
    alihadd res.root *.root
~~~


### Killing jobs

alien.py ps возвращает список запущенных jobs, однако, этот список, похоже, показывает не masterjob, идентификаторы которых отображаются на странице монитора в столбце PID, а все subjobs, на странице монитора показано лишь их число.

Поэтому правильно взять список masterjobs с сайта и завершать их.
Вариант, представленный ниже, очень долгий из-за того, что число subjobs огромно, кроме того, команда kill subjobid  не удаляет процесс из списка, а изменяет его статус на 'K', при этом число subjobs непрерывно уменьшается при выполнении скирпта:

~~~python
import os

processes = os.popen("alien.py ps").readlines()
pids = [process.split(' ')[3] for process in processes if process.split(' ')[9] != 'K']
len(pids)
for pid in pids:
    os.system(f'alien.py kill {pid}')
    print (f'{pid} has killed')

~~~

[^1]: [Презентация](file://D:\GoogleDrive\Job\cern\Alice\study\AliceDataFlow.pdf)
