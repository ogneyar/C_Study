
// Пример Ethernet-приложения для контроллера STM32
// Построим простое приложение, на базе которого происходит обмен данными по сети Ethernet. Мы не станем описывать настройки портов, тактирования периферии и прочие детали проекта, которые не имеют непосредственного отношения к приёму/передаче данных по Ethernet. Проект будет построен на базе прилагаемых стандартных библиотек для Ethernet.
// До задания параметров Ethernet следует установить начальные значения. Выполнить это можно следующим образом:

// сброс настроек в начальное состояние Ethernet
ETH_DeInit();

// сбрасываем Ethernet
ETH_SoftwareReset();

// ждём подтверждения, что Ethernet сбросился
while(ETH_GetSoftwareResetStatus()==SET);

 

// Далее задаем свойства сети:

// конфигурируем Ethernet
ETH_StructInit(&ETH_InitStructure);

// Устанавливаем параметры ETH_InitStructure
ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable ;
ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;

// Принимать всё подряд
// ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Enable;

ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
// производим инициализацию параметров
Value = ETH_Init(&ETH_InitStructure, PHY_ADDRESS);

 

// Процедура ETH_Init настраивает не только внутренние регистры контроллера, но и внешнюю микросхему PHY-уровня. Далее должно быть выделено пространство буферов приёма, буферов передачи и для описывающих их дескрипторов.

// Будем использовать стандартные библиотеки, поэтому создаём соответствующие структуры
ETH_InitTypeDef ETH_InitStructure;

// Ethernet DMA работает с дескрипторами — указатели массивов данных, которые следует транслировать (+ дополнительная служебная информация)
// Дескрипторы
ETH_DMADESCTypeDef DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB];
// массив буферов приёма и отправки данных
// на которые будут ссылаться дескрипторы
u8 Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE], Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];

 

// Описать дескрипторы можно вручную, заполнив все адреса и прочую служебную информацию. Но можно выполнить это гораздо быстрее, воспользовавшись библиотечной функцией. В данной ситуации создаём цепочную структуру дескрипторов (дескриптор, который описывает одну область памяти и указывает адрес следующего дескриптора):

// Записываем часть параметров дескрипторов (указатели на буфер + на следующий дескриптор)
ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
// Записываем длину буфера в дескриптор
DMATxDscrTab->ControlBufferSize = 100;

// Устанавливаем дескрипторы для приёма
ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);
DMARxDscrTab->ControlBufferSize = ETH_MAX_PACKET_SIZE | (1<<14);

// Функции в качестве аргументов имеют начальные адреса дескрипторов, начальные адреса буферов данных и длину одного буфера. Аналогичные библиотечные функции имеются для организации кольцевой структуры дескрипторов.
// Итак, дескрипторы описаны, свойства сети заданы, запускаем Ethernet:

// Разрешаем приём
DMARxDscrTab->Status = ETH_DMARxDesc_OWN;

// Запускаем Ethernet
ETH_Start();

// Как принимать сообщение. Первое, что мы должны сделать — проверить до манипуляций принадлежность дескриптора. Если область памяти отдана приложению, в ней находится принятая по Ethernet информация. В нашем приложении всего одна область памяти, отданная под приём, поэтому требуется отключение и включение приёма по каналам DMA для корректной работы. В приложении нас интересуют байты №№20 и 21 в принятом пакете. Мы их считываем и отдаём дескриптор и буфер в ведение DMA.

// Проверяем, кому принадлежит дескриптор — процессору или
if ((DMARxDscrTab->Status&ETH_DMARxDesc_OWN) == 0)
{
// Чтобы успешно принимать, следует сначала отключить приём (либо успевать обрабатывать все буферы, чтобы DMA — не подвешивался).
ETH_DMAReceptionCmd(DISABLE);
// считываем яркость. Она находится в 20 и 21-м байте посылки (место может быть и иным)
Bright_Rx = Rx_Buff[0][20]+(Rx_Buff[0][21]<<8);
// отдаём дескриптор в DMA Ethernet
DMARxDscrTab->Status = ETH_DMARxDesc_OWN;
// разрешаем приём
ETH_DMAReceptionCmd(ENABLE);
}
// Отправка данных аналогична приёму, с той лишь разницей, что при предыдущей отправке мог возникнуть набор ошибок, которые следует аннулировать.

// а прошлая посылка отправлена?Если отправлена, то
if ((DMATxDscrTab->Status&ETH_DMARxDesc_OWN) == 0)
{
// Чтобы успешно отправлять, следует сначала отключить передачу (либо успевать обрабатывать все буферы, чтобы DMA не подвешивался).
ETH_DMATransmissionCmd(DISABLE);
//помещаем данные по яркости в 20 и 21-й байт посылки
Tx_Buff[0][20] = Bright_Tx&0xFF;
Tx_Buff[0][21] = Bright_Tx>>8;
// отдаём дескриптор в DMA Ethernet
DMATxDscrTab->Status = ETH_DMARxDesc_OWN | ETH_DMATxDesc_TCH | ETH_DMATxDesc_TTSE | ETH_DMATxDesc_LS | ETH_DMATxDesc_FS;
// разрешаем передачу
ETH_DMATransmissionCmd(ENABLE);
}
// В отправляемый пакет помещаем данные. В нашем случае это два байта со смещением 20 и 21 от начала пакета и передаём дескриптор в распоряжение DMA.

// Где же MAC-адреса?
// Действительно, в предыдущем примере мы настроили приёмную часть Ethernet таким образом, чтобы контроллер получал все сообщения. Именно поэтому мы не формировали пакетов. Как это делать, показано ниже:

// адрес назначения МАССОВАЯ РАССЫЛКА
Tx_Buff[0][0]=0xff;
Tx_Buff[0][1]=0xff;
Tx_Buff[0][2]=0xff;
Tx_Buff[0][3]=0xff;
Tx_Buff[0][4]=0xff;
Tx_Buff[0][5]=0xff;
// адрес источника
Tx_Buff[0][6]=0x17;
Tx_Buff[0][7]=0x11;
Tx_Buff[0][8]=0x12;
Tx_Buff[0][9]=0x13;
Tx_Buff[0][10]=0x14;
Tx_Buff[0][11]=0x15;
// длина пакета
Tx_Buff[0][6]=0x0;
Tx_Buff[0][7]=0x55;

// Мы самостоятельно заполняем преамбулу пакета аналогично структуре, описанной в разделе «Ethernet в STM32». При этом мы не заполняем синхронизирующий пакет с командой начала данных — первые восемь байт, а так же контрольную сумму CRC32, которая будет добавлена автоматически.
// Если мы собираемся принимать пакеты определённой структуры, то мы можем ввести фильтрацию. Далее показан вариант настройки Ethernet периферии с организацией фильтрации по адресу источника.

// Настройка фильтра приёма
ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
ETH_InitStructure.ETH_SourceAddrFilter = ETH_SourceAddrFilter_Normal_Enable;
ETH_InitStructure.ETH_DestinationAddrFilter = ETH_DestinationAddrFilter_Normal;
// Далее необходимо настроить фильтр. В качестве фильтра воспользуемся MACAddress1:
//настраиваем MAC-адрес устройства
Mac_addr0[0]=0x0;
Mac_addr0[1]=0x55;
Mac_addr0[2]=0x12;
Mac_addr0[3]=0x13;
Mac_addr0[4]=0x14;
Mac_addr0[5]=0x17;

ETH_MACAddressConfig(ETH_MAC_Address1, Mac_addr0);
ETH_MACAddressFilterConfig(ETH_MAC_Address1,ETH_MAC_AddressFilter_SA);
ETH_MACAddressPerfectFilterCmd(ETH_MAC_Address1, ENABLE);

// Обратите внимание, что первые 2 байта фильтра Mac_addr0[0] и Mac_addr0[1] обозначают длину принимаемого пакета. Кроме того, каждые 4 бита фильтра можно индивидуально подключать/отключать, что позволяет гибче фильтровать приёмные сообщения.