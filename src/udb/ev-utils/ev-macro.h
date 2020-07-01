/******************************************************************************
  * @file File Name : ev-macro.h
  * Description     : Useful C language macroses
  * @project name	: ev-utils
  * @version 		: ev-utils 1.2
  *
 ******************************************************************************
 @author			: Evgeny Voropaev
 @company			: Emmet, LLC
 @original project	: Torock-TC
 @Date of Creation	: 09.04.2020
 ***************************************************************************/

// Define to prevent recursive inclusion -------------------------------------
#ifndef __EV_MACRO_H
#define __EV_MACRO_H
#ifdef __cplusplus
 extern "C" {
#endif

//*****************************************************************************
//       SUBSWITCH macros
//*****************************************************************************
#define SUBSWITCH(x) switch(x)
#define SUBCASE	case
#define SUBDEFAULT default

//*****************************************************************************
//       Macros about data actuality
//*****************************************************************************
#define ACTUAL 1U
#define NOTACTUAL 0U
#define ACT ACTUAL
#define NOACT NOTACTUAL

//declaration for swap function invoke for newlib implementation
#define	htonl(jj) __htonl(jj)
#define	htons(kk) __htons(kk)
#define	ntohl(mm) __ntohl(mm)
#define	ntohs(pp) __ntohs(pp)

//*****************************************************************************
//       Working with big-endian buffers									 //
//*****************************************************************************
#define getBEI16(bu)	\
( (uint16_t)(bu)[0] << 8 | 	\
    	 (uint16_t)(bu)[1]		\
)								\

#define getBEI32(bu) \
( (uint32_t)(bu)[0] << 24 |	\
  (uint32_t)(bu)[1] << 16 |	\
  (uint32_t)(bu)[2] << 8  |	\
  (uint32_t)(bu)[3] 			\
)								\

#define getBEI16shiftBuf(bu)	\
(  ( (bu) = (bu)+2 ),				\
   ( (uint16_t)(bu)[-2] << 8 | 	\
    	 (uint16_t)(bu)[-1]		\
   )								\
)

#define getBEI32shiftBuf(bu) \
(  ( (bu) = (bu)+4 ),				\
   ( (uint32_t)(bu)[-4] << 24 |	\
     (uint32_t)(bu)[-3] << 16 |	\
     (uint32_t)(bu)[-2] << 8  |	\
	 (uint32_t)(bu)[-1] 			\
   )								\
)

//*****************************************************************************
//       Working with litle-endian buffers									 //
//*****************************************************************************
#define getLEI16(bu)	\
( (uint16_t)(bu)[1] << 8 | 	\
    	 (uint16_t)(bu)[0]		\
)								\

#define getLEI32(bu) \
( (uint32_t)(bu)[3] << 24 |	\
  (uint32_t)(bu)[2] << 16 |	\
  (uint32_t)(bu)[1] << 8  |	\
  (uint32_t)(bu)[0] 			\
)								\

#define getLEI16shiftBuf(bu)	\
(  ( (bu) = (bu)+2 ),				\
   ( (uint16_t)(bu)[-1] << 8 | 	\
    	 (uint16_t)(bu)[-2]		\
   )								\
)

#define getLEI32shiftBuf(bu) \
(  ( (bu) = (bu)+4 ),				\
   ( (uint32_t)(bu)[-1] << 24 |	\
     (uint32_t)(bu)[-2] << 16 |	\
     (uint32_t)(bu)[-3] << 8  |	\
	 (uint32_t)(bu)[-4] 			\
   )								\
)

//*****************************************************************************
//       Macros for working with Cortex-M
//*****************************************************************************
#define EV_CORTM_ICSR_VECTACTIVE_MASK		( 0xFFUL )
#define EV_CORTM_IsITnow	( portNVIC_INT_CTRL_REG & EV_CORTM_ICSR_VECTACTIVE_MASK )

#ifdef __cplusplus
}
#endif
#endif //__EV_MACRO_H

//*********************** (C) COPYRIGHT Emmet, LLC for Torock-TC project *****END OF FILE***
