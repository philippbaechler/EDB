#include "PE_Types.h"
#include "MOT_LEFT.h"
#include "MOT_RIGHT.h"
#include "LED_RED.h"
#include "FRTOS1.h"

#include "SIG.h"


#include "MotionController.h"
#include "Serial.h"
#include "RTOS.h"

#include "Servos.h"

#include "WAIT.h"

#include "SERIAL_UART.h"

#include "MOT_LEFT_DIR.h"
#include "MOT_LEFT_M1.h"
#include "MOT_RGHT_NSLEEP.h"
#include "MOT_LEFT_M0.h"
#include "MOT_LEFT_NSLEEP.h"
#include "MOT_RIGHT_DIR.h"
#include "MOT_RIGHT_M0.h"
#include "MOT_RIGHT_M1.h"


MOT_FSMData motionController;

/*
 * Here you can set the new maximal speed. It's called when the PI changes States.
 * */
void MOT_SetMaxSpeed(int16_t master_speed_period){
	motionController.master_speed_period = master_speed_period;
}

void MOT_ChangeState(MOT_StateKinds newState){
	motionController.state = newState;
}

void MOT_SetSpeed(){
	motionController.actual_common_period = MOT_GetPeriodByAcclerationCounter(motionController.accleration_counter);
	motionController.motorLeft.actual_period = ((uint32_t)MOT_GetPeriodByAcclerationCounter(motionController.accleration_counter)*127)/motionController.differential;
	motionController.motorRight.actual_period = ((uint32_t)MOT_GetPeriodByAcclerationCounter(motionController.accleration_counter)*(motionController.differential))/127;

	MOT_LEFT_SetPeriodUS(motionController.motorLeft.actual_period);
	MOT_LEFT_SetDutyUS(motionController.motorLeft.actual_period-DUTY);


	MOT_RIGHT_SetPeriodUS(motionController.motorRight.actual_period);
	MOT_RIGHT_SetDutyUS(motionController.motorRight.actual_period-DUTY);
}

/*
 * New function for setting the steps for deceleration
 * */
void MOT_SetTicksUntilStop(){
	switch(motionController.accleration_counter){
	case	0	:	motionController.steps_for_decleration =	0	;	break;
	case	1	:	motionController.steps_for_decleration =	0	;	break;
	case	2	:	motionController.steps_for_decleration =	0	;	break;
	case	3	:	motionController.steps_for_decleration =	1	;	break;
	case	4	:	motionController.steps_for_decleration =	1	;	break;
	case	5	:	motionController.steps_for_decleration =	2	;	break;
	case	6	:	motionController.steps_for_decleration =	2	;	break;
	case	7	:	motionController.steps_for_decleration =	3	;	break;
	case	8	:	motionController.steps_for_decleration =	4	;	break;
	case	9	:	motionController.steps_for_decleration =	5	;	break;
	case	10	:	motionController.steps_for_decleration =	5	;	break;
	case	11	:	motionController.steps_for_decleration =	6	;	break;
	case	12	:	motionController.steps_for_decleration =	8	;	break;
	case	13	:	motionController.steps_for_decleration =	9	;	break;
	case	14	:	motionController.steps_for_decleration =	10	;	break;
	case	15	:	motionController.steps_for_decleration =	11	;	break;
	case	16	:	motionController.steps_for_decleration =	13	;	break;
	case	17	:	motionController.steps_for_decleration =	14	;	break;
	case	18	:	motionController.steps_for_decleration =	16	;	break;
	case	19	:	motionController.steps_for_decleration =	17	;	break;
	case	20	:	motionController.steps_for_decleration =	19	;	break;
	case	21	:	motionController.steps_for_decleration =	21	;	break;
	case	22	:	motionController.steps_for_decleration =	23	;	break;
	case	23	:	motionController.steps_for_decleration =	24	;	break;
	case	24	:	motionController.steps_for_decleration =	26	;	break;
	case	25	:	motionController.steps_for_decleration =	29	;	break;
	case	26	:	motionController.steps_for_decleration =	31	;	break;
	case	27	:	motionController.steps_for_decleration =	33	;	break;
	case	28	:	motionController.steps_for_decleration =	35	;	break;
	case	29	:	motionController.steps_for_decleration =	38	;	break;
	case	30	:	motionController.steps_for_decleration =	40	;	break;
	case	31	:	motionController.steps_for_decleration =	43	;	break;
	case	32	:	motionController.steps_for_decleration =	45	;	break;
	case	33	:	motionController.steps_for_decleration =	48	;	break;
	case	34	:	motionController.steps_for_decleration =	51	;	break;
	case	35	:	motionController.steps_for_decleration =	54	;	break;
	case	36	:	motionController.steps_for_decleration =	56	;	break;
	case	37	:	motionController.steps_for_decleration =	59	;	break;
	case	38	:	motionController.steps_for_decleration =	63	;	break;
	case	39	:	motionController.steps_for_decleration =	66	;	break;
	case	40	:	motionController.steps_for_decleration =	69	;	break;
	case	41	:	motionController.steps_for_decleration =	72	;	break;
	case	42	:	motionController.steps_for_decleration =	76	;	break;
	case	43	:	motionController.steps_for_decleration =	79	;	break;
	case	44	:	motionController.steps_for_decleration =	83	;	break;
	case	45	:	motionController.steps_for_decleration =	86	;	break;
	case	46	:	motionController.steps_for_decleration =	90	;	break;
	case	47	:	motionController.steps_for_decleration =	94	;	break;
	case	48	:	motionController.steps_for_decleration =	98	;	break;
	case	49	:	motionController.steps_for_decleration =	101	;	break;
	case	50	:	motionController.steps_for_decleration =	105	;	break;
	case	51	:	motionController.steps_for_decleration =	110	;	break;
	case	52	:	motionController.steps_for_decleration =	114	;	break;
	case	53	:	motionController.steps_for_decleration =	118	;	break;
	case	54	:	motionController.steps_for_decleration =	122	;	break;
	case	55	:	motionController.steps_for_decleration =	127	;	break;
	case	56	:	motionController.steps_for_decleration =	131	;	break;
	case	57	:	motionController.steps_for_decleration =	136	;	break;
	case	58	:	motionController.steps_for_decleration =	140	;	break;
	case	59	:	motionController.steps_for_decleration =	145	;	break;
	case	60	:	motionController.steps_for_decleration =	150	;	break;
	case	61	:	motionController.steps_for_decleration =	155	;	break;
	case	62	:	motionController.steps_for_decleration =	159	;	break;
	case	63	:	motionController.steps_for_decleration =	164	;	break;
	case	64	:	motionController.steps_for_decleration =	170	;	break;
	case	65	:	motionController.steps_for_decleration =	175	;	break;
	case	66	:	motionController.steps_for_decleration =	180	;	break;
	case	67	:	motionController.steps_for_decleration =	185	;	break;
	case	68	:	motionController.steps_for_decleration =	191	;	break;
	case	69	:	motionController.steps_for_decleration =	196	;	break;
	case	70	:	motionController.steps_for_decleration =	202	;	break;
	case	71	:	motionController.steps_for_decleration =	207	;	break;
	case	72	:	motionController.steps_for_decleration =	213	;	break;
	case	73	:	motionController.steps_for_decleration =	219	;	break;
	case	74	:	motionController.steps_for_decleration =	225	;	break;
	case	75	:	motionController.steps_for_decleration =	230	;	break;
	case	76	:	motionController.steps_for_decleration =	236	;	break;
	case	77	:	motionController.steps_for_decleration =	243	;	break;
	case	78	:	motionController.steps_for_decleration =	249	;	break;
	case	79	:	motionController.steps_for_decleration =	255	;	break;
	case	80	:	motionController.steps_for_decleration =	261	;	break;
	case	81	:	motionController.steps_for_decleration =	268	;	break;
	case	82	:	motionController.steps_for_decleration =	274	;	break;
	case	83	:	motionController.steps_for_decleration =	281	;	break;
	case	84	:	motionController.steps_for_decleration =	287	;	break;
	case	85	:	motionController.steps_for_decleration =	294	;	break;
	case	86	:	motionController.steps_for_decleration =	301	;	break;
	case	87	:	motionController.steps_for_decleration =	308	;	break;
	case	88	:	motionController.steps_for_decleration =	314	;	break;
	case	89	:	motionController.steps_for_decleration =	321	;	break;
	case	90	:	motionController.steps_for_decleration =	329	;	break;
	case	91	:	motionController.steps_for_decleration =	336	;	break;
	case	92	:	motionController.steps_for_decleration =	343	;	break;
	case	93	:	motionController.steps_for_decleration =	350	;	break;
	case	94	:	motionController.steps_for_decleration =	358	;	break;
	case	95	:	motionController.steps_for_decleration =	365	;	break;
	case	96	:	motionController.steps_for_decleration =	373	;	break;
	case	97	:	motionController.steps_for_decleration =	380	;	break;
	case	98	:	motionController.steps_for_decleration =	388	;	break;
	case	99	:	motionController.steps_for_decleration =	396	;	break;
	case	100	:	motionController.steps_for_decleration =	404	;	break;
	case	101	:	motionController.steps_for_decleration =	411	;	break;
	case	102	:	motionController.steps_for_decleration =	419	;	break;
	case	103	:	motionController.steps_for_decleration =	428	;	break;
	case	104	:	motionController.steps_for_decleration =	436	;	break;
	case	105	:	motionController.steps_for_decleration =	444	;	break;
	case	106	:	motionController.steps_for_decleration =	452	;	break;
	case	107	:	motionController.steps_for_decleration =	461	;	break;
	case	108	:	motionController.steps_for_decleration =	469	;	break;
	case	109	:	motionController.steps_for_decleration =	478	;	break;
	case	110	:	motionController.steps_for_decleration =	486	;	break;
	case	111	:	motionController.steps_for_decleration =	495	;	break;
	case	112	:	motionController.steps_for_decleration =	504	;	break;
	case	113	:	motionController.steps_for_decleration =	513	;	break;
	case	114	:	motionController.steps_for_decleration =	521	;	break;
	case	115	:	motionController.steps_for_decleration =	530	;	break;
	case	116	:	motionController.steps_for_decleration =	540	;	break;
	case	117	:	motionController.steps_for_decleration =	549	;	break;
	case	118	:	motionController.steps_for_decleration =	558	;	break;
	case	119	:	motionController.steps_for_decleration =	567	;	break;
	case	120	:	motionController.steps_for_decleration =	577	;	break;
	case	121	:	motionController.steps_for_decleration =	586	;	break;
	case	122	:	motionController.steps_for_decleration =	596	;	break;
	case	123	:	motionController.steps_for_decleration =	605	;	break;
	case	124	:	motionController.steps_for_decleration =	615	;	break;
	case	125	:	motionController.steps_for_decleration =	625	;	break;
	case	126	:	motionController.steps_for_decleration =	635	;	break;
	case	127	:	motionController.steps_for_decleration =	644	;	break;
	case	128	:	motionController.steps_for_decleration =	654	;	break;
	case	129	:	motionController.steps_for_decleration =	665	;	break;
	case	130	:	motionController.steps_for_decleration =	675	;	break;
	case	131	:	motionController.steps_for_decleration =	685	;	break;
	case	132	:	motionController.steps_for_decleration =	695	;	break;
	case	133	:	motionController.steps_for_decleration =	706	;	break;
	case	134	:	motionController.steps_for_decleration =	716	;	break;
	case	135	:	motionController.steps_for_decleration =	727	;	break;
	case	136	:	motionController.steps_for_decleration =	737	;	break;
	case	137	:	motionController.steps_for_decleration =	748	;	break;
	case	138	:	motionController.steps_for_decleration =	759	;	break;
	case	139	:	motionController.steps_for_decleration =	770	;	break;
	case	140	:	motionController.steps_for_decleration =	780	;	break;
	case	141	:	motionController.steps_for_decleration =	791	;	break;
	case	142	:	motionController.steps_for_decleration =	803	;	break;
	case	143	:	motionController.steps_for_decleration =	814	;	break;
	case	144	:	motionController.steps_for_decleration =	825	;	break;
	case	145	:	motionController.steps_for_decleration =	836	;	break;
	case	146	:	motionController.steps_for_decleration =	848	;	break;
	case	147	:	motionController.steps_for_decleration =	859	;	break;
	case	148	:	motionController.steps_for_decleration =	871	;	break;
	case	149	:	motionController.steps_for_decleration =	882	;	break;
	case	150	:	motionController.steps_for_decleration =	894	;	break;
	case	151	:	motionController.steps_for_decleration =	906	;	break;
	case	152	:	motionController.steps_for_decleration =	918	;	break;
	case	153	:	motionController.steps_for_decleration =	929	;	break;
	case	154	:	motionController.steps_for_decleration =	941	;	break;
	case	155	:	motionController.steps_for_decleration =	954	;	break;
	case	156	:	motionController.steps_for_decleration =	966	;	break;
	case	157	:	motionController.steps_for_decleration =	978	;	break;
	case	158	:	motionController.steps_for_decleration =	990	;	break;
	case	159	:	motionController.steps_for_decleration =	1003	;	break;
	case	160	:	motionController.steps_for_decleration =	1015	;	break;
	case	161	:	motionController.steps_for_decleration =	1028	;	break;
	case	162	:	motionController.steps_for_decleration =	1040	;	break;
	case	163	:	motionController.steps_for_decleration =	1053	;	break;
	case	164	:	motionController.steps_for_decleration =	1066	;	break;
	case	165	:	motionController.steps_for_decleration =	1079	;	break;
	case	166	:	motionController.steps_for_decleration =	1091	;	break;
	case	167	:	motionController.steps_for_decleration =	1104	;	break;
	case	168	:	motionController.steps_for_decleration =	1118	;	break;
	case	169	:	motionController.steps_for_decleration =	1131	;	break;
	case	170	:	motionController.steps_for_decleration =	1144	;	break;
	case	171	:	motionController.steps_for_decleration =	1157	;	break;
	case	172	:	motionController.steps_for_decleration =	1171	;	break;
	case	173	:	motionController.steps_for_decleration =	1184	;	break;
	case	174	:	motionController.steps_for_decleration =	1198	;	break;
	case	175	:	motionController.steps_for_decleration =	1211	;	break;
	case	176	:	motionController.steps_for_decleration =	1225	;	break;
	case	177	:	motionController.steps_for_decleration =	1239	;	break;
	case	178	:	motionController.steps_for_decleration =	1253	;	break;
	case	179	:	motionController.steps_for_decleration =	1266	;	break;
	case	180	:	motionController.steps_for_decleration =	1280	;	break;
	case	181	:	motionController.steps_for_decleration =	1295	;	break;
	case	182	:	motionController.steps_for_decleration =	1309	;	break;
	case	183	:	motionController.steps_for_decleration =	1323	;	break;
	case	184	:	motionController.steps_for_decleration =	1337	;	break;
	case	185	:	motionController.steps_for_decleration =	1352	;	break;
	case	186	:	motionController.steps_for_decleration =	1366	;	break;
	case	187	:	motionController.steps_for_decleration =	1381	;	break;
	case	188	:	motionController.steps_for_decleration =	1395	;	break;
	case	189	:	motionController.steps_for_decleration =	1410	;	break;
	case	190	:	motionController.steps_for_decleration =	1425	;	break;
	case	191	:	motionController.steps_for_decleration =	1440	;	break;
	case	192	:	motionController.steps_for_decleration =	1454	;	break;
	case	193	:	motionController.steps_for_decleration =	1469	;	break;
	case	194	:	motionController.steps_for_decleration =	1485	;	break;
	case	195	:	motionController.steps_for_decleration =	1500	;	break;
	case	196	:	motionController.steps_for_decleration =	1515	;	break;
	case	197	:	motionController.steps_for_decleration =	1530	;	break;
	case	198	:	motionController.steps_for_decleration =	1546	;	break;
	case	199	:	motionController.steps_for_decleration =	1561	;	break;
	case	200	:	motionController.steps_for_decleration =	1577	;	break;
	case	201	:	motionController.steps_for_decleration =	1592	;	break;
	case	202	:	motionController.steps_for_decleration =	1608	;	break;
	case	203	:	motionController.steps_for_decleration =	1624	;	break;
	case	204	:	motionController.steps_for_decleration =	1640	;	break;
	case	205	:	motionController.steps_for_decleration =	1655	;	break;
	case	206	:	motionController.steps_for_decleration =	1671	;	break;
	case	207	:	motionController.steps_for_decleration =	1688	;	break;
	case	208	:	motionController.steps_for_decleration =	1704	;	break;
	case	209	:	motionController.steps_for_decleration =	1720	;	break;
	case	210	:	motionController.steps_for_decleration =	1736	;	break;
	case	211	:	motionController.steps_for_decleration =	1753	;	break;
	case	212	:	motionController.steps_for_decleration =	1769	;	break;
	case	213	:	motionController.steps_for_decleration =	1786	;	break;
	case	214	:	motionController.steps_for_decleration =	1802	;	break;
	case	215	:	motionController.steps_for_decleration =	1819	;	break;
	case	216	:	motionController.steps_for_decleration =	1836	;	break;
	case	217	:	motionController.steps_for_decleration =	1853	;	break;
	case	218	:	motionController.steps_for_decleration =	1869	;	break;
	case	219	:	motionController.steps_for_decleration =	1886	;	break;
	case	220	:	motionController.steps_for_decleration =	1904	;	break;
	case	221	:	motionController.steps_for_decleration =	1921	;	break;
	case	222	:	motionController.steps_for_decleration =	1938	;	break;
	case	223	:	motionController.steps_for_decleration =	1955	;	break;
	case	224	:	motionController.steps_for_decleration =	1973	;	break;
	case	225	:	motionController.steps_for_decleration =	1990	;	break;
	case	226	:	motionController.steps_for_decleration =	2008	;	break;
	case	227	:	motionController.steps_for_decleration =	2025	;	break;
	case	228	:	motionController.steps_for_decleration =	2043	;	break;
	case	229	:	motionController.steps_for_decleration =	2061	;	break;
	case	230	:	motionController.steps_for_decleration =	2079	;	break;
	case	231	:	motionController.steps_for_decleration =	2096	;	break;
	case	232	:	motionController.steps_for_decleration =	2114	;	break;
	case	233	:	motionController.steps_for_decleration =	2133	;	break;
	case	234	:	motionController.steps_for_decleration =	2151	;	break;
	case	235	:	motionController.steps_for_decleration =	2169	;	break;
	case	236	:	motionController.steps_for_decleration =	2187	;	break;
	case	237	:	motionController.steps_for_decleration =	2206	;	break;
	case	238	:	motionController.steps_for_decleration =	2224	;	break;
	case	239	:	motionController.steps_for_decleration =	2243	;	break;
	case	240	:	motionController.steps_for_decleration =	2261	;	break;
	case	241	:	motionController.steps_for_decleration =	2280	;	break;
	case	242	:	motionController.steps_for_decleration =	2299	;	break;
	case	243	:	motionController.steps_for_decleration =	2318	;	break;
	case	244	:	motionController.steps_for_decleration =	2336	;	break;
	case	245	:	motionController.steps_for_decleration =	2355	;	break;
	case	246	:	motionController.steps_for_decleration =	2375	;	break;
	case	247	:	motionController.steps_for_decleration =	2394	;	break;
	case	248	:	motionController.steps_for_decleration =	2413	;	break;
	case	249	:	motionController.steps_for_decleration =	2432	;	break;
	case	250	:	motionController.steps_for_decleration =	2452	;	break;
	case	251	:	motionController.steps_for_decleration =	2471	;	break;
	case	252	:	motionController.steps_for_decleration =	2491	;	break;
	case	253	:	motionController.steps_for_decleration =	2510	;	break;
	case	254	:	motionController.steps_for_decleration =	2530	;	break;
	case	255	:	motionController.steps_for_decleration =	2550	;	break;
	case	256	:	motionController.steps_for_decleration =	2570	;	break;
	case	257	:	motionController.steps_for_decleration =	2589	;	break;
	case	258	:	motionController.steps_for_decleration =	2609	;	break;
	case	259	:	motionController.steps_for_decleration =	2630	;	break;
	case	260	:	motionController.steps_for_decleration =	2650	;	break;
	case	261	:	motionController.steps_for_decleration =	2670	;	break;
	case	262	:	motionController.steps_for_decleration =	2690	;	break;
	case	263	:	motionController.steps_for_decleration =	2711	;	break;
	case	264	:	motionController.steps_for_decleration =	2731	;	break;
	case	265	:	motionController.steps_for_decleration =	2752	;	break;
	case	266	:	motionController.steps_for_decleration =	2772	;	break;
	case	267	:	motionController.steps_for_decleration =	2793	;	break;
	case	268	:	motionController.steps_for_decleration =	2814	;	break;
	case	269	:	motionController.steps_for_decleration =	2835	;	break;
	case	270	:	motionController.steps_for_decleration =	2855	;	break;
	case	271	:	motionController.steps_for_decleration =	2876	;	break;
	case	272	:	motionController.steps_for_decleration =	2898	;	break;
	case	273	:	motionController.steps_for_decleration =	2919	;	break;
	case	274	:	motionController.steps_for_decleration =	2940	;	break;
	case	275	:	motionController.steps_for_decleration =	2961	;	break;
	case	276	:	motionController.steps_for_decleration =	2983	;	break;
	case	277	:	motionController.steps_for_decleration =	3004	;	break;
	case	278	:	motionController.steps_for_decleration =	3026	;	break;
	case	279	:	motionController.steps_for_decleration =	3047	;	break;
	case	280	:	motionController.steps_for_decleration =	3069	;	break;
	case	281	:	motionController.steps_for_decleration =	3091	;	break;
	case	282	:	motionController.steps_for_decleration =	3113	;	break;
	case	283	:	motionController.steps_for_decleration =	3134	;	break;
	case	284	:	motionController.steps_for_decleration =	3156	;	break;
	case	285	:	motionController.steps_for_decleration =	3179	;	break;
	case	286	:	motionController.steps_for_decleration =	3201	;	break;
	case	287	:	motionController.steps_for_decleration =	3223	;	break;
	case	288	:	motionController.steps_for_decleration =	3245	;	break;
	case	289	:	motionController.steps_for_decleration =	3268	;	break;
	case	290	:	motionController.steps_for_decleration =	3290	;	break;
	case	291	:	motionController.steps_for_decleration =	3313	;	break;
	case	292	:	motionController.steps_for_decleration =	3335	;	break;
	case	293	:	motionController.steps_for_decleration =	3358	;	break;
	case	294	:	motionController.steps_for_decleration =	3381	;	break;
	case	295	:	motionController.steps_for_decleration =	3404	;	break;
	case	296	:	motionController.steps_for_decleration =	3426	;	break;
	case	297	:	motionController.steps_for_decleration =	3449	;	break;
	case	298	:	motionController.steps_for_decleration =	3473	;	break;
	case	299	:	motionController.steps_for_decleration =	3496	;	break;
	case	300	:	motionController.steps_for_decleration =	3519	;	break;
	case	301	:	motionController.steps_for_decleration =	3542	;	break;
	case	302	:	motionController.steps_for_decleration =	3566	;	break;
	case	303	:	motionController.steps_for_decleration =	3589	;	break;
	case	304	:	motionController.steps_for_decleration =	3613	;	break;
	case	305	:	motionController.steps_for_decleration =	3636	;	break;
	case	306	:	motionController.steps_for_decleration =	3660	;	break;
	case	307	:	motionController.steps_for_decleration =	3684	;	break;
	case	308	:	motionController.steps_for_decleration =	3708	;	break;
	case	309	:	motionController.steps_for_decleration =	3731	;	break;
	case	310	:	motionController.steps_for_decleration =	3755	;	break;
	case	311	:	motionController.steps_for_decleration =	3780	;	break;
	case	312	:	motionController.steps_for_decleration =	3804	;	break;
	case	313	:	motionController.steps_for_decleration =	3828	;	break;
	case	314	:	motionController.steps_for_decleration =	3852	;	break;
	case	315	:	motionController.steps_for_decleration =	3877	;	break;
	case	316	:	motionController.steps_for_decleration =	3901	;	break;
	case	317	:	motionController.steps_for_decleration =	3926	;	break;
	case	318	:	motionController.steps_for_decleration =	3950	;	break;
	case	319	:	motionController.steps_for_decleration =	3975	;	break;
	case	320	:	motionController.steps_for_decleration =	4000	;	break;
	case	321	:	motionController.steps_for_decleration =	4025	;	break;
	case	322	:	motionController.steps_for_decleration =	4049	;	break;
	case	323	:	motionController.steps_for_decleration =	4074	;	break;
	case	324	:	motionController.steps_for_decleration =	4100	;	break;
	case	325	:	motionController.steps_for_decleration =	4125	;	break;
	case	326	:	motionController.steps_for_decleration =	4150	;	break;
	case	327	:	motionController.steps_for_decleration =	4175	;	break;
	case	328	:	motionController.steps_for_decleration =	4201	;	break;
	case	329	:	motionController.steps_for_decleration =	4226	;	break;
	case	330	:	motionController.steps_for_decleration =	4252	;	break;
	}
}

/*
 * New function for get the periodtime
 * */
uint16_t MOT_GetPeriodByAcclerationCounter(uint16_t aclerationCounter){

	uint16_t period = motionController.max_common_period;

	switch(aclerationCounter){
	case	0	:	period =	65000	;	break;
	case	1	:	period =	43333	;	break;
	case	2	:	period =	32500	;	break;
	case	3	:	period =	26000	;	break;
	case	4	:	period =	21667	;	break;
	case	5	:	period =	18571	;	break;
	case	6	:	period =	16250	;	break;
	case	7	:	period =	14444	;	break;
	case	8	:	period =	13000	;	break;
	case	9	:	period =	11818	;	break;
	case	10	:	period =	10833	;	break;
	case	11	:	period =	10000	;	break;
	case	12	:	period =	9286	;	break;
	case	13	:	period =	8667	;	break;
	case	14	:	period =	8125	;	break;
	case	15	:	period =	7647	;	break;
	case	16	:	period =	7222	;	break;
	case	17	:	period =	6842	;	break;
	case	18	:	period =	6500	;	break;
	case	19	:	period =	6190	;	break;
	case	20	:	period =	5909	;	break;
	case	21	:	period =	5652	;	break;
	case	22	:	period =	5417	;	break;
	case	23	:	period =	5200	;	break;
	case	24	:	period =	5000	;	break;
	case	25	:	period =	4815	;	break;
	case	26	:	period =	4643	;	break;
	case	27	:	period =	4483	;	break;
	case	28	:	period =	4333	;	break;
	case	29	:	period =	4194	;	break;
	case	30	:	period =	4063	;	break;
	case	31	:	period =	3939	;	break;
	case	32	:	period =	3824	;	break;
	case	33	:	period =	3714	;	break;
	case	34	:	period =	3611	;	break;
	case	35	:	period =	3514	;	break;
	case	36	:	period =	3421	;	break;
	case	37	:	period =	3333	;	break;
	case	38	:	period =	3250	;	break;
	case	39	:	period =	3171	;	break;
	case	40	:	period =	3095	;	break;
	case	41	:	period =	3023	;	break;
	case	42	:	period =	2955	;	break;
	case	43	:	period =	2889	;	break;
	case	44	:	period =	2826	;	break;
	case	45	:	period =	2766	;	break;
	case	46	:	period =	2708	;	break;
	case	47	:	period =	2653	;	break;
	case	48	:	period =	2600	;	break;
	case	49	:	period =	2549	;	break;
	case	50	:	period =	2500	;	break;
	case	51	:	period =	2453	;	break;
	case	52	:	period =	2407	;	break;
	case	53	:	period =	2364	;	break;
	case	54	:	period =	2321	;	break;
	case	55	:	period =	2281	;	break;
	case	56	:	period =	2241	;	break;
	case	57	:	period =	2203	;	break;
	case	58	:	period =	2167	;	break;
	case	59	:	period =	2131	;	break;
	case	60	:	period =	2097	;	break;
	case	61	:	period =	2063	;	break;
	case	62	:	period =	2031	;	break;
	case	63	:	period =	2000	;	break;
	case	64	:	period =	1970	;	break;
	case	65	:	period =	1940	;	break;
	case	66	:	period =	1912	;	break;
	case	67	:	period =	1884	;	break;
	case	68	:	period =	1857	;	break;
	case	69	:	period =	1831	;	break;
	case	70	:	period =	1806	;	break;
	case	71	:	period =	1781	;	break;
	case	72	:	period =	1757	;	break;
	case	73	:	period =	1733	;	break;
	case	74	:	period =	1711	;	break;
	case	75	:	period =	1688	;	break;
	case	76	:	period =	1667	;	break;
	case	77	:	period =	1646	;	break;
	case	78	:	period =	1625	;	break;
	case	79	:	period =	1605	;	break;
	case	80	:	period =	1585	;	break;
	case	81	:	period =	1566	;	break;
	case	82	:	period =	1548	;	break;
	case	83	:	period =	1529	;	break;
	case	84	:	period =	1512	;	break;
	case	85	:	period =	1494	;	break;
	case	86	:	period =	1477	;	break;
	case	87	:	period =	1461	;	break;
	case	88	:	period =	1444	;	break;
	case	89	:	period =	1429	;	break;
	case	90	:	period =	1413	;	break;
	case	91	:	period =	1398	;	break;
	case	92	:	period =	1383	;	break;
	case	93	:	period =	1368	;	break;
	case	94	:	period =	1354	;	break;
	case	95	:	period =	1340	;	break;
	case	96	:	period =	1327	;	break;
	case	97	:	period =	1313	;	break;
	case	98	:	period =	1300	;	break;
	case	99	:	period =	1287	;	break;
	case	100	:	period =	1275	;	break;
	case	101	:	period =	1262	;	break;
	case	102	:	period =	1250	;	break;
	case	103	:	period =	1238	;	break;
	case	104	:	period =	1226	;	break;
	case	105	:	period =	1215	;	break;
	case	106	:	period =	1204	;	break;
	case	107	:	period =	1193	;	break;
	case	108	:	period =	1182	;	break;
	case	109	:	period =	1171	;	break;
	case	110	:	period =	1161	;	break;
	case	111	:	period =	1150	;	break;
	case	112	:	period =	1140	;	break;
	case	113	:	period =	1130	;	break;
	case	114	:	period =	1121	;	break;
	case	115	:	period =	1111	;	break;
	case	116	:	period =	1102	;	break;
	case	117	:	period =	1092	;	break;
	case	118	:	period =	1083	;	break;
	case	119	:	period =	1074	;	break;
	case	120	:	period =	1066	;	break;
	case	121	:	period =	1057	;	break;
	case	122	:	period =	1048	;	break;
	case	123	:	period =	1040	;	break;
	case	124	:	period =	1032	;	break;
	case	125	:	period =	1024	;	break;
	case	126	:	period =	1016	;	break;
	case	127	:	period =	1008	;	break;
	case	128	:	period =	1000	;	break;
	case	129	:	period =	992	;	break;
	case	130	:	period =	985	;	break;
	case	131	:	period =	977	;	break;
	case	132	:	period =	970	;	break;
	case	133	:	period =	963	;	break;
	case	134	:	period =	956	;	break;
	case	135	:	period =	949	;	break;
	case	136	:	period =	942	;	break;
	case	137	:	period =	935	;	break;
	case	138	:	period =	929	;	break;
	case	139	:	period =	922	;	break;
	case	140	:	period =	915	;	break;
	case	141	:	period =	909	;	break;
	case	142	:	period =	903	;	break;
	case	143	:	period =	897	;	break;
	case	144	:	period =	890	;	break;
	case	145	:	period =	884	;	break;
	case	146	:	period =	878	;	break;
	case	147	:	period =	872	;	break;
	case	148	:	period =	867	;	break;
	case	149	:	period =	861	;	break;
	case	150	:	period =	855	;	break;
	case	151	:	period =	850	;	break;
	case	152	:	period =	844	;	break;
	case	153	:	period =	839	;	break;
	case	154	:	period =	833	;	break;
	case	155	:	period =	828	;	break;
	case	156	:	period =	823	;	break;
	case	157	:	period =	818	;	break;
	case	158	:	period =	813	;	break;
	case	159	:	period =	807	;	break;
	case	160	:	period =	802	;	break;
	case	161	:	period =	798	;	break;
	case	162	:	period =	793	;	break;
	case	163	:	period =	788	;	break;
	case	164	:	period =	783	;	break;
	case	165	:	period =	778	;	break;
	case	166	:	period =	774	;	break;
	case	167	:	period =	769	;	break;
	case	168	:	period =	765	;	break;
	case	169	:	period =	760	;	break;
	case	170	:	period =	756	;	break;
	case	171	:	period =	751	;	break;
	case	172	:	period =	747	;	break;
	case	173	:	period =	743	;	break;
	case	174	:	period =	739	;	break;
	case	175	:	period =	734	;	break;
	case	176	:	period =	730	;	break;
	case	177	:	period =	726	;	break;
	case	178	:	period =	722	;	break;
	case	179	:	period =	718	;	break;
	case	180	:	period =	714	;	break;
	case	181	:	period =	710	;	break;
	case	182	:	period =	707	;	break;
	case	183	:	period =	703	;	break;
	case	184	:	period =	699	;	break;
	case	185	:	period =	695	;	break;
	case	186	:	period =	691	;	break;
	case	187	:	period =	688	;	break;
	case	188	:	period =	684	;	break;
	case	189	:	period =	681	;	break;
	case	190	:	period =	677	;	break;
	case	191	:	period =	674	;	break;
	case	192	:	period =	670	;	break;
	case	193	:	period =	667	;	break;
	case	194	:	period =	663	;	break;
	case	195	:	period =	660	;	break;
	case	196	:	period =	657	;	break;
	case	197	:	period =	653	;	break;
	case	198	:	period =	650	;	break;
	case	199	:	period =	647	;	break;
	case	200	:	period =	644	;	break;
	case	201	:	period =	640	;	break;
	case	202	:	period =	637	;	break;
	case	203	:	period =	634	;	break;
	case	204	:	period =	631	;	break;
	case	205	:	period =	628	;	break;
	case	206	:	period =	625	;	break;
	case	207	:	period =	622	;	break;
	case	208	:	period =	619	;	break;
	case	209	:	period =	616	;	break;
	case	210	:	period =	613	;	break;
	case	211	:	period =	610	;	break;
	case	212	:	period =	607	;	break;
	case	213	:	period =	605	;	break;
	case	214	:	period =	602	;	break;
	case	215	:	period =	599	;	break;
	case	216	:	period =	596	;	break;
	case	217	:	period =	594	;	break;
	case	218	:	period =	591	;	break;
	case	219	:	period =	588	;	break;
	case	220	:	period =	586	;	break;
	case	221	:	period =	583	;	break;
	case	222	:	period =	580	;	break;
	case	223	:	period =	578	;	break;
	case	224	:	period =	575	;	break;
	case	225	:	period =	573	;	break;
	case	226	:	period =	570	;	break;
	case	227	:	period =	568	;	break;
	case	228	:	period =	565	;	break;
	case	229	:	period =	563	;	break;
	case	230	:	period =	560	;	break;
	case	231	:	period =	558	;	break;
	case	232	:	period =	556	;	break;
	case	233	:	period =	553	;	break;
	case	234	:	period =	551	;	break;
	case	235	:	period =	549	;	break;
	case	236	:	period =	546	;	break;
	case	237	:	period =	544	;	break;
	case	238	:	period =	542	;	break;
	case	239	:	period =	539	;	break;
	case	240	:	period =	537	;	break;
	case	241	:	period =	535	;	break;
	case	242	:	period =	533	;	break;
	case	243	:	period =	531	;	break;
	case	244	:	period =	528	;	break;
	case	245	:	period =	526	;	break;
	case	246	:	period =	524	;	break;
	case	247	:	period =	522	;	break;
	case	248	:	period =	520	;	break;
	case	249	:	period =	518	;	break;
	case	250	:	period =	516	;	break;
	case	251	:	period =	514	;	break;
	case	252	:	period =	512	;	break;
	case	253	:	period =	510	;	break;
	case	254	:	period =	508	;	break;
	case	255	:	period =	506	;	break;
	case	256	:	period =	504	;	break;
	case	257	:	period =	502	;	break;
	case	258	:	period =	500	;	break;
	case	259	:	period =	498	;	break;
	case	260	:	period =	496	;	break;
	case	261	:	period =	494	;	break;
	case	262	:	period =	492	;	break;
	case	263	:	period =	491	;	break;
	case	264	:	period =	489	;	break;
	case	265	:	period =	487	;	break;
	case	266	:	period =	485	;	break;
	case	267	:	period =	483	;	break;
	case	268	:	period =	481	;	break;
	case	269	:	period =	480	;	break;
	case	270	:	period =	478	;	break;
	case	271	:	period =	476	;	break;
	case	272	:	period =	474	;	break;
	case	273	:	period =	473	;	break;
	case	274	:	period =	471	;	break;
	case	275	:	period =	469	;	break;
	case	276	:	period =	468	;	break;
	case	277	:	period =	466	;	break;
	case	278	:	period =	464	;	break;
	case	279	:	period =	463	;	break;
	case	280	:	period =	461	;	break;
	case	281	:	period =	459	;	break;
	case	282	:	period =	458	;	break;
	case	283	:	period =	456	;	break;
	case	284	:	period =	455	;	break;
	case	285	:	period =	453	;	break;
	case	286	:	period =	451	;	break;
	case	287	:	period =	450	;	break;
	case	288	:	period =	448	;	break;
	case	289	:	period =	447	;	break;
	case	290	:	period =	445	;	break;
	case	291	:	period =	444	;	break;
	case	292	:	period =	442	;	break;
	case	293	:	period =	441	;	break;
	case	294	:	period =	439	;	break;
	case	295	:	period =	438	;	break;
	case	296	:	period =	436	;	break;
	case	297	:	period =	435	;	break;
	case	298	:	period =	433	;	break;
	case	299	:	period =	432	;	break;
	case	300	:	period =	430	;	break;
	case	301	:	period =	429	;	break;
	case	302	:	period =	428	;	break;
	case	303	:	period =	426	;	break;
	case	304	:	period =	425	;	break;
	case	305	:	period =	423	;	break;
	case	306	:	period =	422	;	break;
	case	307	:	period =	421	;	break;
	case	308	:	period =	419	;	break;
	case	309	:	period =	418	;	break;
	case	310	:	period =	417	;	break;
	case	311	:	period =	415	;	break;
	case	312	:	period =	414	;	break;
	case	313	:	period =	413	;	break;
	case	314	:	period =	411	;	break;
	case	315	:	period =	410	;	break;
	case	316	:	period =	409	;	break;
	case	317	:	period =	408	;	break;
	case	318	:	period =	406	;	break;
	case	319	:	period =	405	;	break;
	case	320	:	period =	404	;	break;
	case	321	:	period =	402	;	break;
	case	322	:	period =	401	;	break;
	case	323	:	period =	400	;	break;
	case	324	:	period =	399	;	break;
	case	325	:	period =	398	;	break;
	case	326	:	period =	396	;	break;
	case	327	:	period =	395	;	break;
	case	328	:	period =	394	;	break;
	case	329	:	period =	393	;	break;
	case	330	:	period =	392	;	break;
	}

	return period;
}

void MOT_CalcualteNOfSteps(){
	uint16_t newSteps = (10000 / motionController.actual_common_period); // works only when actual_common_period > 10000, we've got here an error in our distance measuring. Since we've normally have periods < 1000, this shoulden't be a problem!
	motionController.step_count = motionController.step_count + newSteps;

	if (motionController.step_count >= motionController.step_count_target && motionController.step_count_target != 0){
		SER_SendEvent(); /* sending an event if we reach the number of steps to the PI */
		motionController.step_count = 0;
		motionController.step_count_target = 0;
	}

	if (motionController.steps_left_until_stop > motionController.steps_for_decleration/* && motionController.steps_left_until_stop != 0*/){
		motionController.steps_left_until_stop = motionController.steps_left_until_stop - newSteps;
	}
}

void MOT_Process(){

	switch(motionController.state){
		case MOT_FSM_STOP:

			motionController.steps_left_until_stop = 0; // There are some steps left to do.. (ca. 50)

			LED_RED_Off();

			motionController.actual_common_period = motionController.max_common_period;

			if(motionController.actual_common_period > motionController.target_common_period){
				motionController.state = MOT_FSM_ACCEL;
			}

			break;

		case MOT_FSM_ACCEL:

			MOT_CalcualteNOfSteps();

			MOT_LEFT_Enable();
			MOT_RIGHT_Enable();
			motionController.accleration_counter++;

			MOT_SetSpeed();
			MOT_SetTicksUntilStop();

			/* Check if we reached the target speed */
			if(motionController.actual_common_period <= motionController.target_common_period){
				motionController.state = MOT_FSM_RUN;
				LED_RED_On();
			}

			/* Check if we have to start the deceleration for a stop */
			if(motionController.steps_for_decleration >= motionController.steps_left_until_stop && motionController.steps_left_until_stop != 0){
				motionController.master_speed_period = motionController.max_common_period;
				motionController.target_common_period = motionController.master_speed_period;
				motionController.state = MOT_FSM_DECEL;
			}

			break;

		case MOT_FSM_RUN:

			MOT_CalcualteNOfSteps();

			LED_RED_Off();

			MOT_SetSpeed();
			MOT_SetTicksUntilStop();

			if((motionController.actual_common_period-motionController.target_common_period) > 0){
				motionController.state = MOT_FSM_ACCEL;
			}
			else if((motionController.target_common_period-motionController.actual_common_period) > 0){
				motionController.state = MOT_FSM_DECEL;
			}

			/* Check if we have to start the deceleration */
			if(motionController.steps_for_decleration >= motionController.steps_left_until_stop && motionController.steps_left_until_stop != 0){
				motionController.master_speed_period = motionController.max_common_period;
				motionController.target_common_period = motionController.master_speed_period;
				motionController.state = MOT_FSM_DECEL;
			}

			break;

		case MOT_FSM_DECEL:

			MOT_CalcualteNOfSteps();
			motionController.accleration_counter--;

			MOT_SetSpeed();
			MOT_SetTicksUntilStop();

			if(motionController.actual_common_period >= motionController.target_common_period){

				if (motionController.target_common_period == motionController.max_common_period){

					motionController.running = FALSE;

					motionController.state = MOT_FSM_STOP;
					MOT_LEFT_Disable();
					MOT_RIGHT_Disable();
				}
				else{
					motionController.state = MOT_FSM_RUN;
				}
				LED_RED_On();
			}
			else if(motionController.accleration_counter == 0){

				motionController.running = FALSE;

				motionController.state = MOT_FSM_STOP;
				MOT_LEFT_Disable();
				MOT_RIGHT_Disable();
			}

			break;
	}
}

void vMotionControlTask(){

	for(;;){
		// have to be executed exactly every 10 ms
		RTOS_Wait(10);

		if(motionController.running){
			MOT_Process();

			uint16_t i = motionController.actual_common_period;

//			RTOS_Wait(10);
		}
		else{
//			FRTOS1_taskYIELD();
		}
	}
}

/*
 * Initializes the MotionController. Different values could be defined here. e.g. minimal period time
 * */
void MOT_Init(void){

	MOT_LEFT_NSLEEP_ClrVal();
	MOT_LEFT_DIR_SetVal();
	MOT_LEFT_M0_SetVal();
	MOT_LEFT_M1_SetVal();

	MOT_RGHT_NSLEEP_SetVal();
	MOT_RIGHT_DIR_SetVal();
	MOT_RIGHT_M0_SetVal();
	MOT_RIGHT_M1_SetVal();

	SRV_Init(); // maybe we should initialize the servos when a button is pressed, like a command?

	motionController.running = FALSE;
	motionController.state = MOT_FSM_STOP;
	motionController.max_common_period = 65000;
	motionController.min_common_period = 200;
	motionController.differential = 127;

	motionController.target_common_period = motionController.max_common_period;

	MOT_LEFT_Disable();
	MOT_RIGHT_Disable();

	RTOS_AddTask(vMotionControlTask, "MOT", 3);
}
