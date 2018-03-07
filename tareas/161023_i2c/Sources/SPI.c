
/**
	\file SPI.c
	\brief
	\author Alexis Andalón / Alejandro Canale
	\date	28/3/2016
 */
#include "DataTypeDefinitions.h"
#include "SPI.h"

/**Los siguientes arreglos globales de direcciones cambian entre los diferentes canales*/

volatile uint32* SPIx_MCR[3]={&SPI0_MCR, &SPI1_MCR, &SPI2_MCR};/**arreglo global de direcciones del canal del SPIx_MCR*/
volatile uint32* SPI_CTAR[3]={&SPI0_CTAR0, &SPI1_CTAR0, &SPI2_CTAR0};/**arreglo global de direcciones del canal del SPIx_CTARn*/

/** it enable the clock module of the SPI by modifying the MDIS bits*/
void SPI_enable(SPI_ChannelType channel)
{
	*SPIx_MCR[channel] &=~SPI_MCR_MDIS_MASK;//Module Disable esta en el registro SPIx_MCR // pongo un 0
}

/** It activate the clock gating*/
static void SPI_clk(SPI_ChannelType channel)
{
	switch(channel)/** Selecting the SPI for clock enabling*/
		{
		case SPI_0: /** SPI_0 is selected*/
			SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK;
			break;
		case SPI_1: /** SPI_1 is selected*/
			SIM_SCGC6 |= SIM_SCGC6_SPI1_MASK;
			break;
		case SPI_2: /** SPI_2 is selected*/
			SIM_SCGC3 |= SIM_SCGC3_SPI2_MASK;
			break;
		default: /** all is selected */
			SIM_SCGC6=0xFFFFFFFF;
			SIM_SCGC3=0xFFFFFFFF;
		}// end switch
}


/**It configure the SPI as a master or slave depending on the value of masterOrslave*/
static void SPI_setMaster(SPI_ChannelType channel, SPI_MasterType masterOrslave)
{
	//*SPIx_MCR[channel]|=SPI_MCR_MSTR(x);

	if(SPI_MASTER==masterOrslave)
		*SPIx_MCR[channel]|=SPI_MCR_MSTR_MASK;/**SPI_MCR_MSTR_MASK es set 1 en el registro 32 (0x80000000)*/
	else
		*SPIx_MCR[channel]&=~(SPI_MCR_MSTR_MASK);/**Es lo mismo que &=0x7FFFFFFF;*/
}

/**It activate the TX and RX FIFOs of the SPI depending on the value of enableOrdisable*/
static void SPI_fIFO(SPI_ChannelType channel, SPI_EnableFIFOType enableOrDisable)
{
	if( SPI_DISABLE_FIFO == enableOrDisable )
		*SPIx_MCR[channel] &= ~(SPI_MCR_DIS_TXF_MASK|SPI_MCR_DIS_RXF_MASK);/**Se activa la transmision y recepción al mismo tiempo*/
	else
		*SPIx_MCR[channel] |= (SPI_MCR_DIS_TXF_MASK|SPI_MCR_DIS_RXF_MASK);
}

/**It selects the clock polarity depending on the value of cpol*/
void SPI_clockPolarity(SPI_ChannelType channel, SPI_PolarityType cpol)
{
	if (SPI_LOW_POLARITY == cpol)
		*SPI_CTAR[channel] &= ~SPI_CTAR_CPOL_MASK; /**&=0xFBFFFFFF;*/
	else
		*SPI_CTAR[channel] |= SPI_CTAR_CPOL_MASK; //0x04000000;//if   SPI_HIGH_POLARITY
}

/**It selects the frame size depending on the value of frameSize and the macros that are defined above*/
static void SPI_frameSize(SPI_ChannelType channel, uint32 frameSize)
{
	//uint32 fmsz=((frameSize-1)&0x1F)<<27;
	*SPI_CTAR[channel] &= 0x07FFFFFF;
	//*SPI_CTAR[channel] |= fmsz;
	*SPI_CTAR[channel] |= frameSize;
}

/**It selects the clock phase depending on the value of cpha*/
static void SPI_clockPhase(SPI_ChannelType channel, SPI_PhaseType cpha){

	if(SPI_LOW_PHASE == cpha)
		*SPI_CTAR[channel] |= SPI_CTAR_CPHA_MASK;
	else
		*SPI_CTAR[channel] &= ~SPI_CTAR_CPHA_MASK;
}

/**It selects the baud rate depending on the value of baudRate and the macros that are defined above*/
static void SPI_baudRate(SPI_ChannelType channel, uint32 baudRate){

/**unidad de teransferencia de datos*/
		*SPI_CTAR[channel] |= baudRate;

}

/**It selects if MSB or LSM bits is first transmitted*/
static void SPI_mSBFirst(SPI_ChannelType channel, SPI_LSMorMSBType msb){

	if(SPI_MSB == msb)
			*SPI_CTAR[channel] &= ~SPI_CTAR_LSBFE_MASK;
	else
			*SPI_CTAR[channel] |= SPI_CTAR_LSBFE_MASK;
}

/**It stars the SPI transmission by modifying the value of HALT bit*/
void SPI_startTranference(SPI_ChannelType channel){

	*SPIx_MCR[channel] &=~SPI_MCR_HALT_MASK;
}
/**It stops the SPI transmission by modifying the value of HALT bit*/
void SPI_stopTranference(SPI_ChannelType channel){

	*SPIx_MCR[channel] |=SPI_MCR_HALT_MASK;

}
/**It transmits the information contained in data*/
void SPI_sendOneByte(uint8 Data){
	SPI0_PUSHR = (Data);
	while(0 == (SPI0_SR & SPI_SR_TCF_MASK));
	SPI0_SR |= SPI_SR_TCF_MASK;
}

/**It configures the SPI for transmission, this function as arguments receives a pointer to a constant structure where are all
 * the configuration parameters*/
void SPI_init(const SPI_ConfigType* SPI_Config)/**Pointer to constant, el modificador const califica al tipo de dato, por seguridad de que no te escriban, solo lectura*/
{
	SPI_clk(SPI_Config->SPI_Channel);/**Se utiliza el operador this para acceder a los miembros de la estructura, que en este caso es una estructura apuntador*/
	GPIO_clockGating(SPI_Config->GPIOForSPI.GPIO_portName);
	GPIO_pinControlRegister(SPI_Config->GPIOForSPI.GPIO_portName,SPI_Config->GPIOForSPI.SPI_clk,&(SPI_Config->pinConttrolRegisterPORTD));
	GPIO_pinControlRegister(SPI_Config->GPIOForSPI.GPIO_portName,SPI_Config->GPIOForSPI.SPI_Sout,&(SPI_Config->pinConttrolRegisterPORTD));
	SPI_setMaster(SPI_Config->SPI_Channel,SPI_Config->SPI_Master);
	SPI_fIFO(SPI_Config->SPI_Channel,SPI_Config->SPI_EnableFIFO);
	SPI_enable(SPI_Config->SPI_Channel);
	SPI_clockPolarity(SPI_Config->SPI_Channel,SPI_Config->SPI_Polarity);
	SPI_frameSize(SPI_Config->SPI_Channel,SPI_Config->frameSize);
	SPI_clockPhase(SPI_Config->SPI_Channel,SPI_Config->SPI_Phase);
	SPI_baudRate(SPI_Config->SPI_Channel,SPI_Config->baudrate);
	SPI_mSBFirst(SPI_Config->SPI_Channel,SPI_MSB);
}
