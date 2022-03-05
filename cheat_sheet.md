    int ProcNum, ProcRank;
    <программный код без использования MPI функций>
    MPI_Init ( &agrc, &argv );
    MPI_Comm_size ( MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank ( MPI_COMM_WORLD, &ProcRank);
    <программный код с использованием MPI функций>
    MPI_Finalize();
#
    int MPI_Send(void *buf, int count, MPI_Datatype type, int dest,
    int tag, MPI_Comm comm),
    где
    - buf – адрес буфера памяти, в котором располагаются данные

    отправляемого сообщения,

    - count – количество элементов данных в сообщении,
    - type - тип элементов данных пересылаемого сообщения,
    - dest - ранг процесса, которому отправляется сообщение,
    - tag - значение-тег, используемое для идентификации сообщений,
    - comm - коммуникатор, в рамках которого выполняется передача данных.
#
    MPI_Datatype        C Datatype
    MPI_BYTE
    MPI_CHAR            signed char
    MPI_DOUBLE          double
    MPI_FLOAT           float
    MPI_INT             int
    MPI_LONG            long
    MPI_LONG_DOUBLE     long double
    MPI_PACKED
    MPI_SHORT           short
    MPI_UNSIGNED_CHAR   unsigned char
    MPI_UNSIGNED        unsigned int
    MPI_UNSIGNED_LONG   unsigned long
    MPI_UNSIGNED_SHORT  unsigned short
#
    int MPI_Recv(void *buf, int count, MPI_Datatype type, int source,
    int tag, MPI_Comm comm, MPI_Status *status),
    где
    - buf, count, type – буфер памяти для приема сообщения, назначение
    каждого отдельного параметра соответствует описанию в MPI_Send,
    - source - ранг процесса, от которого должен быть выполнен прием

    сообщения,

    - tag - тег сообщения, которое должно быть принято для процесса,
    - comm - коммуникатор, в рамках которого выполняется передача данных,
    - status – указатель на структуру данных с информацией о результате

    выполнения операции приема данных.

    Параметр status позволяет определить ряд характеристик принятого сообщения:
    - status.MPI_SOURCE – ранг процесса-отправителя принятого сообщения,
    - status.MPI_TAG - тег принятого сообщения.
    Функция
    MPI_Get_count(MPI_Status *status, MPI_Datatype type, int *count)
#
    Функция MPI_Bcast осуществляет рассылку данных из буфера buf, содержащего count элементов типа type с
    процесса, имеющего номер root, всем процессам, входящим в коммуникатор comm
    
    int MPI_Bcast(void *buf,int count,MPI_Datatype type,int root,MPI_Comm comm),
    где
    - buf, count, type – буфер памяти с отправляемым сообщением (для процесса
    с рангом 0), и для приема сообщений для всех остальных процессов,
    - root - ранг процесса, выполняющего рассылку данных,
    - comm - коммуникатор, в рамках которого выполняется передача данных.
#
    int MPI_Reduce(void *sendbuf, void *recvbuf,int count,MPI_Datatype type,

    MPI_Op op,int root,MPI_Comm comm),

    где
    - sendbuf - буфер памяти с отправляемым сообщением,
    - recvbuf – буфер памяти для результирующего сообщения (только для
    процесса с рангом root),
    - count - количество элементов в сообщениях,
    - type – тип элементов сообщений,
    - op - операция, которая должна быть выполнена над данными,
    - root - ранг процесса, на котором должен быть получен результат,
    - comm - коммуникатор, в рамках которого выполняется операция.

    Операция    Описание
    MPI_MAX     Определение максимального значения
    MPI_MIN     Определение минимального значения
    MPI_SUM     Определение суммы значений
    MPI_PROD    Определение произведения значений
    MPI_LAND    Выполнение логической операции "И" над значениями сообщений
    MPI_BAND    Выполнение битовой операции "И" над значениями сообщений
    MPI_LOR     Выполнение логической операции "ИЛИ" над значениями сообщений
    MPI_BOR     Выполнение битовой операции "ИЛИ" над значениями сообщений
    MPI_LXOR    Выполнение логической операции исключающего "ИЛИ" над значениями сообщений
    MPI_BXOR    Выполнение битовой операции исключающего "ИЛИ" над значениями сообщений
    MPI_MAXLOC  Определение максимальных значений и их индексов
    MPI_MINLOC  Определение минимальных значений и их индексов
#
    int MPI_Barrier(MPI_Comm comm);
#
    • Синхронный (Synchronous) режим состоит в том, что завершение функции отправки сообщения
    происходит только при получении от процесса-получателя подтверждения о начале приема отправленного
    сообщения, отправленное сообщение или полностью принято процессом-получателем или находится в
    состоянии приема,
    • Буферизованный (Buffered) режим предполагает использование дополнительных системных
    буферов для копирования в них отправляемых сообщений; как результат, функция отправки сообщения
    завершается сразу же после копирования сообщения в системный буфер,
    • Режим передачи по готовности (Ready) может быть использован только, если операция приема
    сообщения уже инициирована. Буфер сообщения после завершения функции отправки сообщения может
    быть повторно использован.

    - MPI_Ssend – функция отправки сообщения в синхронном режиме,
    - MPI_Bsend – функция отправки сообщения в буферизованном режиме,
    - MPI_Rsend – функция отправки сообщения в режиме по готовности.

    Для использования буферизованного режима передачи должен быть создан и передан MPI буфер памяти
    для буферизации сообщений – используемая для этого функция имеет вид:
    int MPI_Buffer_attach(void *buf, int size),
    где
    - buf - буфер памяти для буферизации сообщений,
    - size – размер буфера.
    После завершения работы с буфером он должен быть отключен от MPI при помощи функции:
    int MPI_Buffer_detach(void *buf, int *size).
#
    MPI обеспечивает возможность неблокированного выполнения операций передачи данных между двумя
    процессами. Наименование неблокирующих аналогов образуется из названий соответствующих функций
    путем добавления префикса I (Immediate). Список параметров неблокирующих функций содержит весь
    набор параметров исходных функций и один дополнительный параметр request с типом MPI_Request (в
    функции MPI_Irecv отсутствует также параметр status):

    int MPI_Isend(void *buf, int count, MPI_Datatype type, int dest,
    int tag, MPI_Comm comm, MPI_Request *request)
    int MPI_Issend(void *buf, int count, MPI_Datatype type, int dest,
    int tag, MPI_Comm comm, MPI_Request *request)
    int MPI_Ibsend(void *buf, int count, MPI_Datatype type, int dest,
    int tag, MPI_Comm comm, MPI_Request *request)
    int MPI_Irsend(void *buf, int count, MPI_Datatype type, int dest,
    int tag, MPI_Comm comm, MPI_Request *request)
    int MPI_Irecv(void *buf, int count, MPI_Datatype type, int source,
    int tag, MPI_Comm comm, MPI_Request *request)

    int MPI_Test( MPI_Request *request, int *flag, MPI_status *status),
    где
    - request - дескриптор операции, определенный при вызове неблокирующей
    функции,
    - flag - результат проверки (=true, если операция завершена),
    - status - результат выполнения операции обмена (только для завершенной
    операции).

    MPI_Isend(buf,count,type,dest,tag,comm,&request);
    ...
    do {
    ...
    MPI_Test(&request,&flag,&status)
    } while ( !flag );

    Если при выполнении неблокирующей операции окажется, что продолжение вычислений невозможно
    без получения передаваемых данных, то может быть использована блокирующая операция ожидания
    завершения операции:
    int MPI_Wait( MPI_Request *request, MPI_status *status).

    - MPI_Testall - проверка завершения всех перечисленных операций обмена,
    - MPI_Waitall – ожидание завершения всех операций обмена,
    - MPI_Testany - проверка завершения хотя бы одной из перечисленных
    операций обмена,
    - MPI_Waitany – ожидание завершения любой из перечисленных операций
    обмена,
    - MPI_Testsome - проверка завершения каждой из перечисленных операций
    обмена,
    - MPI_Waitsome - ожидание завершения хотя бы одной из перечисленных
    операций обмена и оценка состояния по всем операциям.
#
    int MPI_Sendrecv(void *sbuf,int scount,MPI_Datatype stype,int dest, int stag,
    void *rbuf,int rcount,MPI_Datatype rtype,int source,int rtag,
    MPI_Comm comm, MPI_Status *status),
    где
    - sbuf, scount, stype, dest, stag - параметры передаваемого сообщения,
    - rbuf, rcount, rtype, source, rtag - параметры принимаемого сообщения,
    - comm - коммуникатор, в рамках которого выполняется передача данных,
    - status – структура данных с информацией о результате выполнения операции.

    int MPI_Sendrecv_replace (void *buf, int count, MPI_Datatype type, int dest,
    int stag, int source, int rtag, MPI_Comm comm, MPI_Status* status).

#
    int MPI_Scatter(void *sbuf,int scount,MPI_Datatype stype,
    void *rbuf,int rcount,MPI_Datatype rtype,
    int root, MPI_Comm comm),
    где
    - sbuf, scount, stype - параметры передаваемого сообщения (scount определяет
    количество элементов, передаваемых на каждый процесс),
    - rbuf, rcount, rtype - параметры сообщения, принимаемого в процессах,
    - root – ранг процесса, выполняющего рассылку данных,
    - comm - коммуникатор, в рамках которого выполняется передача данных.
#
    int MPI_Gather(void *sbuf,int scount,MPI_Datatype stype,
    void *rbuf,int rcount,MPI_Datatype rtype,
    int root, MPI_Comm comm),
    где
    - sbuf, scount, stype - параметры передаваемого сообщения,
    - rbuf, rcount, rtype - параметры принимаемого сообщения,
    - root – ранг процесса, выполняющего сбор данных,
    - comm - коммуникатор, в рамках которого выполняется передача данных.
#
    int MPI_Allgather(void *sbuf, int scount, MPI_Datatype stype,
    void *rbuf, int rcount, MPI_Datatype rtype, MPI_Comm comm).
#
    int MPI_Alltoall(void *sbuf,int scount,MPI_Datatype stype,
    void *rbuf,int rcount,MPI_Datatype rtype,MPI_Comm comm),
    где
    - sbuf, scount, stype - параметры передаваемых сообщений,
    - rbuf, rcount, rtype - параметры принимаемых сообщений
    - comm - коммуникатор, в рамках которого выполняется передача данных.