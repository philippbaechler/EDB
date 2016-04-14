#include "PE_Types.h"
#include "MOT_LEFT.h"
#include "MOT_RIGHT.h"
#include "LED_RED.h"

#include "MotionController.h"
#include "Serial.h"
#include "RTOS.h"

#include "SERIAL_UART.h"


MOT_FSMData motionController;

/*
 * Here you can set the new maximal speed. It's called when the PI changes States.
 * */
void MOT_SetMaxSpeed(int16_t master_speed_period){
	motionController.master_speed_period = master_speed_period;
}

/*
 * Here you can set the new speed. It's used when we acclerate or decclerate.
 * */
void MOT_SetSpeed(){

	MOT_Regulate();

	MOT_LEFT_SetPeriodUS(motionController.motorLeft.actual_period);
	MOT_RIGHT_SetPeriodUS(motionController.motorRight.actual_period);
}

void MOT_ChangeState(MOT_StateKinds newState){
	motionController.state = newState;
}

/*
 * Preparation for the PID regulation. At the moment it's just a linear regulation.
 * Should just be called, when the differential-Value is changed.
 * */
void MOT_Regulate(){

	MOT_CalcualteDifferential();
}

/*
 * Returns the period duration for a specific number accleration_value (0-330).
 * */
void MOT_SetPeriod(){

	switch (motionController.accleration_counter){
	case	0	:	motionController.actual_common_period =	65000	;	motionController.steps_for_decleration =	0	;	break;
	case	1	:	motionController.actual_common_period =	43333	;	motionController.steps_for_decleration =	0	;	break;
	case	2	:	motionController.actual_common_period =	32500	;	motionController.steps_for_decleration =	0	;	break;
	case	3	:	motionController.actual_common_period =	26000	;	motionController.steps_for_decleration =	1	;	break;
	case	4	:	motionController.actual_common_period =	21667	;	motionController.steps_for_decleration =	1	;	break;
	case	5	:	motionController.actual_common_period =	18571	;	motionController.steps_for_decleration =	2	;	break;
	case	6	:	motionController.actual_common_period =	16250	;	motionController.steps_for_decleration =	2	;	break;
	case	7	:	motionController.actual_common_period =	14444	;	motionController.steps_for_decleration =	3	;	break;
	case	8	:	motionController.actual_common_period =	13000	;	motionController.steps_for_decleration =	4	;	break;
	case	9	:	motionController.actual_common_period =	11818	;	motionController.steps_for_decleration =	5	;	break;
	case	10	:	motionController.actual_common_period =	10833	;	motionController.steps_for_decleration =	5	;	break;
	case	11	:	motionController.actual_common_period =	10000	;	motionController.steps_for_decleration =	6	;	break;
	case	12	:	motionController.actual_common_period =	9286	;	motionController.steps_for_decleration =	8	;	break;
	case	13	:	motionController.actual_common_period =	8667	;	motionController.steps_for_decleration =	9	;	break;
	case	14	:	motionController.actual_common_period =	8125	;	motionController.steps_for_decleration =	10	;	break;
	case	15	:	motionController.actual_common_period =	7647	;	motionController.steps_for_decleration =	11	;	break;
	case	16	:	motionController.actual_common_period =	7222	;	motionController.steps_for_decleration =	13	;	break;
	case	17	:	motionController.actual_common_period =	6842	;	motionController.steps_for_decleration =	14	;	break;
	case	18	:	motionController.actual_common_period =	6500	;	motionController.steps_for_decleration =	16	;	break;
	case	19	:	motionController.actual_common_period =	6190	;	motionController.steps_for_decleration =	17	;	break;
	case	20	:	motionController.actual_common_period =	5909	;	motionController.steps_for_decleration =	19	;	break;
	case	21	:	motionController.actual_common_period =	5652	;	motionController.steps_for_decleration =	21	;	break;
	case	22	:	motionController.actual_common_period =	5417	;	motionController.steps_for_decleration =	23	;	break;
	case	23	:	motionController.actual_common_period =	5200	;	motionController.steps_for_decleration =	24	;	break;
	case	24	:	motionController.actual_common_period =	5000	;	motionController.steps_for_decleration =	26	;	break;
	case	25	:	motionController.actual_common_period =	4815	;	motionController.steps_for_decleration =	29	;	break;
	case	26	:	motionController.actual_common_period =	4643	;	motionController.steps_for_decleration =	31	;	break;
	case	27	:	motionController.actual_common_period =	4483	;	motionController.steps_for_decleration =	33	;	break;
	case	28	:	motionController.actual_common_period =	4333	;	motionController.steps_for_decleration =	35	;	break;
	case	29	:	motionController.actual_common_period =	4194	;	motionController.steps_for_decleration =	38	;	break;
	case	30	:	motionController.actual_common_period =	4063	;	motionController.steps_for_decleration =	40	;	break;
	case	31	:	motionController.actual_common_period =	3939	;	motionController.steps_for_decleration =	43	;	break;
	case	32	:	motionController.actual_common_period =	3824	;	motionController.steps_for_decleration =	45	;	break;
	case	33	:	motionController.actual_common_period =	3714	;	motionController.steps_for_decleration =	48	;	break;
	case	34	:	motionController.actual_common_period =	3611	;	motionController.steps_for_decleration =	51	;	break;
	case	35	:	motionController.actual_common_period =	3514	;	motionController.steps_for_decleration =	54	;	break;
	case	36	:	motionController.actual_common_period =	3421	;	motionController.steps_for_decleration =	56	;	break;
	case	37	:	motionController.actual_common_period =	3333	;	motionController.steps_for_decleration =	59	;	break;
	case	38	:	motionController.actual_common_period =	3250	;	motionController.steps_for_decleration =	63	;	break;
	case	39	:	motionController.actual_common_period =	3171	;	motionController.steps_for_decleration =	66	;	break;
	case	40	:	motionController.actual_common_period =	3095	;	motionController.steps_for_decleration =	69	;	break;
	case	41	:	motionController.actual_common_period =	3023	;	motionController.steps_for_decleration =	72	;	break;
	case	42	:	motionController.actual_common_period =	2955	;	motionController.steps_for_decleration =	76	;	break;
	case	43	:	motionController.actual_common_period =	2889	;	motionController.steps_for_decleration =	79	;	break;
	case	44	:	motionController.actual_common_period =	2826	;	motionController.steps_for_decleration =	83	;	break;
	case	45	:	motionController.actual_common_period =	2766	;	motionController.steps_for_decleration =	86	;	break;
	case	46	:	motionController.actual_common_period =	2708	;	motionController.steps_for_decleration =	90	;	break;
	case	47	:	motionController.actual_common_period =	2653	;	motionController.steps_for_decleration =	94	;	break;
	case	48	:	motionController.actual_common_period =	2600	;	motionController.steps_for_decleration =	98	;	break;
	case	49	:	motionController.actual_common_period =	2549	;	motionController.steps_for_decleration =	101	;	break;
	case	50	:	motionController.actual_common_period =	2500	;	motionController.steps_for_decleration =	105	;	break;
	case	51	:	motionController.actual_common_period =	2453	;	motionController.steps_for_decleration =	110	;	break;
	case	52	:	motionController.actual_common_period =	2407	;	motionController.steps_for_decleration =	114	;	break;
	case	53	:	motionController.actual_common_period =	2364	;	motionController.steps_for_decleration =	118	;	break;
	case	54	:	motionController.actual_common_period =	2321	;	motionController.steps_for_decleration =	122	;	break;
	case	55	:	motionController.actual_common_period =	2281	;	motionController.steps_for_decleration =	127	;	break;
	case	56	:	motionController.actual_common_period =	2241	;	motionController.steps_for_decleration =	131	;	break;
	case	57	:	motionController.actual_common_period =	2203	;	motionController.steps_for_decleration =	136	;	break;
	case	58	:	motionController.actual_common_period =	2167	;	motionController.steps_for_decleration =	140	;	break;
	case	59	:	motionController.actual_common_period =	2131	;	motionController.steps_for_decleration =	145	;	break;
	case	60	:	motionController.actual_common_period =	2097	;	motionController.steps_for_decleration =	150	;	break;
	case	61	:	motionController.actual_common_period =	2063	;	motionController.steps_for_decleration =	155	;	break;
	case	62	:	motionController.actual_common_period =	2031	;	motionController.steps_for_decleration =	159	;	break;
	case	63	:	motionController.actual_common_period =	2000	;	motionController.steps_for_decleration =	164	;	break;
	case	64	:	motionController.actual_common_period =	1970	;	motionController.steps_for_decleration =	170	;	break;
	case	65	:	motionController.actual_common_period =	1940	;	motionController.steps_for_decleration =	175	;	break;
	case	66	:	motionController.actual_common_period =	1912	;	motionController.steps_for_decleration =	180	;	break;
	case	67	:	motionController.actual_common_period =	1884	;	motionController.steps_for_decleration =	185	;	break;
	case	68	:	motionController.actual_common_period =	1857	;	motionController.steps_for_decleration =	191	;	break;
	case	69	:	motionController.actual_common_period =	1831	;	motionController.steps_for_decleration =	196	;	break;
	case	70	:	motionController.actual_common_period =	1806	;	motionController.steps_for_decleration =	202	;	break;
	case	71	:	motionController.actual_common_period =	1781	;	motionController.steps_for_decleration =	207	;	break;
	case	72	:	motionController.actual_common_period =	1757	;	motionController.steps_for_decleration =	213	;	break;
	case	73	:	motionController.actual_common_period =	1733	;	motionController.steps_for_decleration =	219	;	break;
	case	74	:	motionController.actual_common_period =	1711	;	motionController.steps_for_decleration =	225	;	break;
	case	75	:	motionController.actual_common_period =	1688	;	motionController.steps_for_decleration =	230	;	break;
	case	76	:	motionController.actual_common_period =	1667	;	motionController.steps_for_decleration =	236	;	break;
	case	77	:	motionController.actual_common_period =	1646	;	motionController.steps_for_decleration =	243	;	break;
	case	78	:	motionController.actual_common_period =	1625	;	motionController.steps_for_decleration =	249	;	break;
	case	79	:	motionController.actual_common_period =	1605	;	motionController.steps_for_decleration =	255	;	break;
	case	80	:	motionController.actual_common_period =	1585	;	motionController.steps_for_decleration =	261	;	break;
	case	81	:	motionController.actual_common_period =	1566	;	motionController.steps_for_decleration =	268	;	break;
	case	82	:	motionController.actual_common_period =	1548	;	motionController.steps_for_decleration =	274	;	break;
	case	83	:	motionController.actual_common_period =	1529	;	motionController.steps_for_decleration =	281	;	break;
	case	84	:	motionController.actual_common_period =	1512	;	motionController.steps_for_decleration =	287	;	break;
	case	85	:	motionController.actual_common_period =	1494	;	motionController.steps_for_decleration =	294	;	break;
	case	86	:	motionController.actual_common_period =	1477	;	motionController.steps_for_decleration =	301	;	break;
	case	87	:	motionController.actual_common_period =	1461	;	motionController.steps_for_decleration =	308	;	break;
	case	88	:	motionController.actual_common_period =	1444	;	motionController.steps_for_decleration =	314	;	break;
	case	89	:	motionController.actual_common_period =	1429	;	motionController.steps_for_decleration =	321	;	break;
	case	90	:	motionController.actual_common_period =	1413	;	motionController.steps_for_decleration =	329	;	break;
	case	91	:	motionController.actual_common_period =	1398	;	motionController.steps_for_decleration =	336	;	break;
	case	92	:	motionController.actual_common_period =	1383	;	motionController.steps_for_decleration =	343	;	break;
	case	93	:	motionController.actual_common_period =	1368	;	motionController.steps_for_decleration =	350	;	break;
	case	94	:	motionController.actual_common_period =	1354	;	motionController.steps_for_decleration =	358	;	break;
	case	95	:	motionController.actual_common_period =	1340	;	motionController.steps_for_decleration =	365	;	break;
	case	96	:	motionController.actual_common_period =	1327	;	motionController.steps_for_decleration =	373	;	break;
	case	97	:	motionController.actual_common_period =	1313	;	motionController.steps_for_decleration =	380	;	break;
	case	98	:	motionController.actual_common_period =	1300	;	motionController.steps_for_decleration =	388	;	break;
	case	99	:	motionController.actual_common_period =	1287	;	motionController.steps_for_decleration =	396	;	break;
	case	100	:	motionController.actual_common_period =	1275	;	motionController.steps_for_decleration =	404	;	break;
	case	101	:	motionController.actual_common_period =	1262	;	motionController.steps_for_decleration =	411	;	break;
	case	102	:	motionController.actual_common_period =	1250	;	motionController.steps_for_decleration =	419	;	break;
	case	103	:	motionController.actual_common_period =	1238	;	motionController.steps_for_decleration =	428	;	break;
	case	104	:	motionController.actual_common_period =	1226	;	motionController.steps_for_decleration =	436	;	break;
	case	105	:	motionController.actual_common_period =	1215	;	motionController.steps_for_decleration =	444	;	break;
	case	106	:	motionController.actual_common_period =	1204	;	motionController.steps_for_decleration =	452	;	break;
	case	107	:	motionController.actual_common_period =	1193	;	motionController.steps_for_decleration =	461	;	break;
	case	108	:	motionController.actual_common_period =	1182	;	motionController.steps_for_decleration =	469	;	break;
	case	109	:	motionController.actual_common_period =	1171	;	motionController.steps_for_decleration =	478	;	break;
	case	110	:	motionController.actual_common_period =	1161	;	motionController.steps_for_decleration =	486	;	break;
	case	111	:	motionController.actual_common_period =	1150	;	motionController.steps_for_decleration =	495	;	break;
	case	112	:	motionController.actual_common_period =	1140	;	motionController.steps_for_decleration =	504	;	break;
	case	113	:	motionController.actual_common_period =	1130	;	motionController.steps_for_decleration =	513	;	break;
	case	114	:	motionController.actual_common_period =	1121	;	motionController.steps_for_decleration =	521	;	break;
	case	115	:	motionController.actual_common_period =	1111	;	motionController.steps_for_decleration =	530	;	break;
	case	116	:	motionController.actual_common_period =	1102	;	motionController.steps_for_decleration =	540	;	break;
	case	117	:	motionController.actual_common_period =	1092	;	motionController.steps_for_decleration =	549	;	break;
	case	118	:	motionController.actual_common_period =	1083	;	motionController.steps_for_decleration =	558	;	break;
	case	119	:	motionController.actual_common_period =	1074	;	motionController.steps_for_decleration =	567	;	break;
	case	120	:	motionController.actual_common_period =	1066	;	motionController.steps_for_decleration =	577	;	break;
	case	121	:	motionController.actual_common_period =	1057	;	motionController.steps_for_decleration =	586	;	break;
	case	122	:	motionController.actual_common_period =	1048	;	motionController.steps_for_decleration =	596	;	break;
	case	123	:	motionController.actual_common_period =	1040	;	motionController.steps_for_decleration =	605	;	break;
	case	124	:	motionController.actual_common_period =	1032	;	motionController.steps_for_decleration =	615	;	break;
	case	125	:	motionController.actual_common_period =	1024	;	motionController.steps_for_decleration =	625	;	break;
	case	126	:	motionController.actual_common_period =	1016	;	motionController.steps_for_decleration =	635	;	break;
	case	127	:	motionController.actual_common_period =	1008	;	motionController.steps_for_decleration =	644	;	break;
	case	128	:	motionController.actual_common_period =	1000	;	motionController.steps_for_decleration =	654	;	break;
	case	129	:	motionController.actual_common_period =	992	;	motionController.steps_for_decleration =	665	;	break;
	case	130	:	motionController.actual_common_period =	985	;	motionController.steps_for_decleration =	675	;	break;
	case	131	:	motionController.actual_common_period =	977	;	motionController.steps_for_decleration =	685	;	break;
	case	132	:	motionController.actual_common_period =	970	;	motionController.steps_for_decleration =	695	;	break;
	case	133	:	motionController.actual_common_period =	963	;	motionController.steps_for_decleration =	706	;	break;
	case	134	:	motionController.actual_common_period =	956	;	motionController.steps_for_decleration =	716	;	break;
	case	135	:	motionController.actual_common_period =	949	;	motionController.steps_for_decleration =	727	;	break;
	case	136	:	motionController.actual_common_period =	942	;	motionController.steps_for_decleration =	737	;	break;
	case	137	:	motionController.actual_common_period =	935	;	motionController.steps_for_decleration =	748	;	break;
	case	138	:	motionController.actual_common_period =	929	;	motionController.steps_for_decleration =	759	;	break;
	case	139	:	motionController.actual_common_period =	922	;	motionController.steps_for_decleration =	770	;	break;
	case	140	:	motionController.actual_common_period =	915	;	motionController.steps_for_decleration =	780	;	break;
	case	141	:	motionController.actual_common_period =	909	;	motionController.steps_for_decleration =	791	;	break;
	case	142	:	motionController.actual_common_period =	903	;	motionController.steps_for_decleration =	803	;	break;
	case	143	:	motionController.actual_common_period =	897	;	motionController.steps_for_decleration =	814	;	break;
	case	144	:	motionController.actual_common_period =	890	;	motionController.steps_for_decleration =	825	;	break;
	case	145	:	motionController.actual_common_period =	884	;	motionController.steps_for_decleration =	836	;	break;
	case	146	:	motionController.actual_common_period =	878	;	motionController.steps_for_decleration =	848	;	break;
	case	147	:	motionController.actual_common_period =	872	;	motionController.steps_for_decleration =	859	;	break;
	case	148	:	motionController.actual_common_period =	867	;	motionController.steps_for_decleration =	871	;	break;
	case	149	:	motionController.actual_common_period =	861	;	motionController.steps_for_decleration =	882	;	break;
	case	150	:	motionController.actual_common_period =	855	;	motionController.steps_for_decleration =	894	;	break;
	case	151	:	motionController.actual_common_period =	850	;	motionController.steps_for_decleration =	906	;	break;
	case	152	:	motionController.actual_common_period =	844	;	motionController.steps_for_decleration =	918	;	break;
	case	153	:	motionController.actual_common_period =	839	;	motionController.steps_for_decleration =	929	;	break;
	case	154	:	motionController.actual_common_period =	833	;	motionController.steps_for_decleration =	941	;	break;
	case	155	:	motionController.actual_common_period =	828	;	motionController.steps_for_decleration =	954	;	break;
	case	156	:	motionController.actual_common_period =	823	;	motionController.steps_for_decleration =	966	;	break;
	case	157	:	motionController.actual_common_period =	818	;	motionController.steps_for_decleration =	978	;	break;
	case	158	:	motionController.actual_common_period =	813	;	motionController.steps_for_decleration =	990	;	break;
	case	159	:	motionController.actual_common_period =	807	;	motionController.steps_for_decleration =	1003	;	break;
	case	160	:	motionController.actual_common_period =	802	;	motionController.steps_for_decleration =	1015	;	break;
	case	161	:	motionController.actual_common_period =	798	;	motionController.steps_for_decleration =	1028	;	break;
	case	162	:	motionController.actual_common_period =	793	;	motionController.steps_for_decleration =	1040	;	break;
	case	163	:	motionController.actual_common_period =	788	;	motionController.steps_for_decleration =	1053	;	break;
	case	164	:	motionController.actual_common_period =	783	;	motionController.steps_for_decleration =	1066	;	break;
	case	165	:	motionController.actual_common_period =	778	;	motionController.steps_for_decleration =	1079	;	break;
	case	166	:	motionController.actual_common_period =	774	;	motionController.steps_for_decleration =	1091	;	break;
	case	167	:	motionController.actual_common_period =	769	;	motionController.steps_for_decleration =	1104	;	break;
	case	168	:	motionController.actual_common_period =	765	;	motionController.steps_for_decleration =	1118	;	break;
	case	169	:	motionController.actual_common_period =	760	;	motionController.steps_for_decleration =	1131	;	break;
	case	170	:	motionController.actual_common_period =	756	;	motionController.steps_for_decleration =	1144	;	break;
	case	171	:	motionController.actual_common_period =	751	;	motionController.steps_for_decleration =	1157	;	break;
	case	172	:	motionController.actual_common_period =	747	;	motionController.steps_for_decleration =	1171	;	break;
	case	173	:	motionController.actual_common_period =	743	;	motionController.steps_for_decleration =	1184	;	break;
	case	174	:	motionController.actual_common_period =	739	;	motionController.steps_for_decleration =	1198	;	break;
	case	175	:	motionController.actual_common_period =	734	;	motionController.steps_for_decleration =	1211	;	break;
	case	176	:	motionController.actual_common_period =	730	;	motionController.steps_for_decleration =	1225	;	break;
	case	177	:	motionController.actual_common_period =	726	;	motionController.steps_for_decleration =	1239	;	break;
	case	178	:	motionController.actual_common_period =	722	;	motionController.steps_for_decleration =	1253	;	break;
	case	179	:	motionController.actual_common_period =	718	;	motionController.steps_for_decleration =	1266	;	break;
	case	180	:	motionController.actual_common_period =	714	;	motionController.steps_for_decleration =	1280	;	break;
	case	181	:	motionController.actual_common_period =	710	;	motionController.steps_for_decleration =	1295	;	break;
	case	182	:	motionController.actual_common_period =	707	;	motionController.steps_for_decleration =	1309	;	break;
	case	183	:	motionController.actual_common_period =	703	;	motionController.steps_for_decleration =	1323	;	break;
	case	184	:	motionController.actual_common_period =	699	;	motionController.steps_for_decleration =	1337	;	break;
	case	185	:	motionController.actual_common_period =	695	;	motionController.steps_for_decleration =	1352	;	break;
	case	186	:	motionController.actual_common_period =	691	;	motionController.steps_for_decleration =	1366	;	break;
	case	187	:	motionController.actual_common_period =	688	;	motionController.steps_for_decleration =	1381	;	break;
	case	188	:	motionController.actual_common_period =	684	;	motionController.steps_for_decleration =	1395	;	break;
	case	189	:	motionController.actual_common_period =	681	;	motionController.steps_for_decleration =	1410	;	break;
	case	190	:	motionController.actual_common_period =	677	;	motionController.steps_for_decleration =	1425	;	break;
	case	191	:	motionController.actual_common_period =	674	;	motionController.steps_for_decleration =	1440	;	break;
	case	192	:	motionController.actual_common_period =	670	;	motionController.steps_for_decleration =	1454	;	break;
	case	193	:	motionController.actual_common_period =	667	;	motionController.steps_for_decleration =	1469	;	break;
	case	194	:	motionController.actual_common_period =	663	;	motionController.steps_for_decleration =	1485	;	break;
	case	195	:	motionController.actual_common_period =	660	;	motionController.steps_for_decleration =	1500	;	break;
	case	196	:	motionController.actual_common_period =	657	;	motionController.steps_for_decleration =	1515	;	break;
	case	197	:	motionController.actual_common_period =	653	;	motionController.steps_for_decleration =	1530	;	break;
	case	198	:	motionController.actual_common_period =	650	;	motionController.steps_for_decleration =	1546	;	break;
	case	199	:	motionController.actual_common_period =	647	;	motionController.steps_for_decleration =	1561	;	break;
	case	200	:	motionController.actual_common_period =	644	;	motionController.steps_for_decleration =	1577	;	break;
	case	201	:	motionController.actual_common_period =	640	;	motionController.steps_for_decleration =	1592	;	break;
	case	202	:	motionController.actual_common_period =	637	;	motionController.steps_for_decleration =	1608	;	break;
	case	203	:	motionController.actual_common_period =	634	;	motionController.steps_for_decleration =	1624	;	break;
	case	204	:	motionController.actual_common_period =	631	;	motionController.steps_for_decleration =	1640	;	break;
	case	205	:	motionController.actual_common_period =	628	;	motionController.steps_for_decleration =	1655	;	break;
	case	206	:	motionController.actual_common_period =	625	;	motionController.steps_for_decleration =	1671	;	break;
	case	207	:	motionController.actual_common_period =	622	;	motionController.steps_for_decleration =	1688	;	break;
	case	208	:	motionController.actual_common_period =	619	;	motionController.steps_for_decleration =	1704	;	break;
	case	209	:	motionController.actual_common_period =	616	;	motionController.steps_for_decleration =	1720	;	break;
	case	210	:	motionController.actual_common_period =	613	;	motionController.steps_for_decleration =	1736	;	break;
	case	211	:	motionController.actual_common_period =	610	;	motionController.steps_for_decleration =	1753	;	break;
	case	212	:	motionController.actual_common_period =	607	;	motionController.steps_for_decleration =	1769	;	break;
	case	213	:	motionController.actual_common_period =	605	;	motionController.steps_for_decleration =	1786	;	break;
	case	214	:	motionController.actual_common_period =	602	;	motionController.steps_for_decleration =	1802	;	break;
	case	215	:	motionController.actual_common_period =	599	;	motionController.steps_for_decleration =	1819	;	break;
	case	216	:	motionController.actual_common_period =	596	;	motionController.steps_for_decleration =	1836	;	break;
	case	217	:	motionController.actual_common_period =	594	;	motionController.steps_for_decleration =	1853	;	break;
	case	218	:	motionController.actual_common_period =	591	;	motionController.steps_for_decleration =	1869	;	break;
	case	219	:	motionController.actual_common_period =	588	;	motionController.steps_for_decleration =	1886	;	break;
	case	220	:	motionController.actual_common_period =	586	;	motionController.steps_for_decleration =	1904	;	break;
	case	221	:	motionController.actual_common_period =	583	;	motionController.steps_for_decleration =	1921	;	break;
	case	222	:	motionController.actual_common_period =	580	;	motionController.steps_for_decleration =	1938	;	break;
	case	223	:	motionController.actual_common_period =	578	;	motionController.steps_for_decleration =	1955	;	break;
	case	224	:	motionController.actual_common_period =	575	;	motionController.steps_for_decleration =	1973	;	break;
	case	225	:	motionController.actual_common_period =	573	;	motionController.steps_for_decleration =	1990	;	break;
	case	226	:	motionController.actual_common_period =	570	;	motionController.steps_for_decleration =	2008	;	break;
	case	227	:	motionController.actual_common_period =	568	;	motionController.steps_for_decleration =	2025	;	break;
	case	228	:	motionController.actual_common_period =	565	;	motionController.steps_for_decleration =	2043	;	break;
	case	229	:	motionController.actual_common_period =	563	;	motionController.steps_for_decleration =	2061	;	break;
	case	230	:	motionController.actual_common_period =	560	;	motionController.steps_for_decleration =	2079	;	break;
	case	231	:	motionController.actual_common_period =	558	;	motionController.steps_for_decleration =	2096	;	break;
	case	232	:	motionController.actual_common_period =	556	;	motionController.steps_for_decleration =	2114	;	break;
	case	233	:	motionController.actual_common_period =	553	;	motionController.steps_for_decleration =	2133	;	break;
	case	234	:	motionController.actual_common_period =	551	;	motionController.steps_for_decleration =	2151	;	break;
	case	235	:	motionController.actual_common_period =	549	;	motionController.steps_for_decleration =	2169	;	break;
	case	236	:	motionController.actual_common_period =	546	;	motionController.steps_for_decleration =	2187	;	break;
	case	237	:	motionController.actual_common_period =	544	;	motionController.steps_for_decleration =	2206	;	break;
	case	238	:	motionController.actual_common_period =	542	;	motionController.steps_for_decleration =	2224	;	break;
	case	239	:	motionController.actual_common_period =	539	;	motionController.steps_for_decleration =	2243	;	break;
	case	240	:	motionController.actual_common_period =	537	;	motionController.steps_for_decleration =	2261	;	break;
	case	241	:	motionController.actual_common_period =	535	;	motionController.steps_for_decleration =	2280	;	break;
	case	242	:	motionController.actual_common_period =	533	;	motionController.steps_for_decleration =	2299	;	break;
	case	243	:	motionController.actual_common_period =	531	;	motionController.steps_for_decleration =	2318	;	break;
	case	244	:	motionController.actual_common_period =	528	;	motionController.steps_for_decleration =	2336	;	break;
	case	245	:	motionController.actual_common_period =	526	;	motionController.steps_for_decleration =	2355	;	break;
	case	246	:	motionController.actual_common_period =	524	;	motionController.steps_for_decleration =	2375	;	break;
	case	247	:	motionController.actual_common_period =	522	;	motionController.steps_for_decleration =	2394	;	break;
	case	248	:	motionController.actual_common_period =	520	;	motionController.steps_for_decleration =	2413	;	break;
	case	249	:	motionController.actual_common_period =	518	;	motionController.steps_for_decleration =	2432	;	break;
	case	250	:	motionController.actual_common_period =	516	;	motionController.steps_for_decleration =	2452	;	break;
	case	251	:	motionController.actual_common_period =	514	;	motionController.steps_for_decleration =	2471	;	break;
	case	252	:	motionController.actual_common_period =	512	;	motionController.steps_for_decleration =	2491	;	break;
	case	253	:	motionController.actual_common_period =	510	;	motionController.steps_for_decleration =	2510	;	break;
	case	254	:	motionController.actual_common_period =	508	;	motionController.steps_for_decleration =	2530	;	break;
	case	255	:	motionController.actual_common_period =	506	;	motionController.steps_for_decleration =	2550	;	break;
	case	256	:	motionController.actual_common_period =	504	;	motionController.steps_for_decleration =	2570	;	break;
	case	257	:	motionController.actual_common_period =	502	;	motionController.steps_for_decleration =	2589	;	break;
	case	258	:	motionController.actual_common_period =	500	;	motionController.steps_for_decleration =	2609	;	break;
	case	259	:	motionController.actual_common_period =	498	;	motionController.steps_for_decleration =	2630	;	break;
	case	260	:	motionController.actual_common_period =	496	;	motionController.steps_for_decleration =	2650	;	break;
	case	261	:	motionController.actual_common_period =	494	;	motionController.steps_for_decleration =	2670	;	break;
	case	262	:	motionController.actual_common_period =	492	;	motionController.steps_for_decleration =	2690	;	break;
	case	263	:	motionController.actual_common_period =	491	;	motionController.steps_for_decleration =	2711	;	break;
	case	264	:	motionController.actual_common_period =	489	;	motionController.steps_for_decleration =	2731	;	break;
	case	265	:	motionController.actual_common_period =	487	;	motionController.steps_for_decleration =	2752	;	break;
	case	266	:	motionController.actual_common_period =	485	;	motionController.steps_for_decleration =	2772	;	break;
	case	267	:	motionController.actual_common_period =	483	;	motionController.steps_for_decleration =	2793	;	break;
	case	268	:	motionController.actual_common_period =	481	;	motionController.steps_for_decleration =	2814	;	break;
	case	269	:	motionController.actual_common_period =	480	;	motionController.steps_for_decleration =	2835	;	break;
	case	270	:	motionController.actual_common_period =	478	;	motionController.steps_for_decleration =	2855	;	break;
	case	271	:	motionController.actual_common_period =	476	;	motionController.steps_for_decleration =	2876	;	break;
	case	272	:	motionController.actual_common_period =	474	;	motionController.steps_for_decleration =	2898	;	break;
	case	273	:	motionController.actual_common_period =	473	;	motionController.steps_for_decleration =	2919	;	break;
	case	274	:	motionController.actual_common_period =	471	;	motionController.steps_for_decleration =	2940	;	break;
	case	275	:	motionController.actual_common_period =	469	;	motionController.steps_for_decleration =	2961	;	break;
	case	276	:	motionController.actual_common_period =	468	;	motionController.steps_for_decleration =	2983	;	break;
	case	277	:	motionController.actual_common_period =	466	;	motionController.steps_for_decleration =	3004	;	break;
	case	278	:	motionController.actual_common_period =	464	;	motionController.steps_for_decleration =	3026	;	break;
	case	279	:	motionController.actual_common_period =	463	;	motionController.steps_for_decleration =	3047	;	break;
	case	280	:	motionController.actual_common_period =	461	;	motionController.steps_for_decleration =	3069	;	break;
	case	281	:	motionController.actual_common_period =	459	;	motionController.steps_for_decleration =	3091	;	break;
	case	282	:	motionController.actual_common_period =	458	;	motionController.steps_for_decleration =	3113	;	break;
	case	283	:	motionController.actual_common_period =	456	;	motionController.steps_for_decleration =	3134	;	break;
	case	284	:	motionController.actual_common_period =	455	;	motionController.steps_for_decleration =	3156	;	break;
	case	285	:	motionController.actual_common_period =	453	;	motionController.steps_for_decleration =	3179	;	break;
	case	286	:	motionController.actual_common_period =	451	;	motionController.steps_for_decleration =	3201	;	break;
	case	287	:	motionController.actual_common_period =	450	;	motionController.steps_for_decleration =	3223	;	break;
	case	288	:	motionController.actual_common_period =	448	;	motionController.steps_for_decleration =	3245	;	break;
	case	289	:	motionController.actual_common_period =	447	;	motionController.steps_for_decleration =	3268	;	break;
	case	290	:	motionController.actual_common_period =	445	;	motionController.steps_for_decleration =	3290	;	break;
	case	291	:	motionController.actual_common_period =	444	;	motionController.steps_for_decleration =	3313	;	break;
	case	292	:	motionController.actual_common_period =	442	;	motionController.steps_for_decleration =	3335	;	break;
	case	293	:	motionController.actual_common_period =	441	;	motionController.steps_for_decleration =	3358	;	break;
	case	294	:	motionController.actual_common_period =	439	;	motionController.steps_for_decleration =	3381	;	break;
	case	295	:	motionController.actual_common_period =	438	;	motionController.steps_for_decleration =	3404	;	break;
	case	296	:	motionController.actual_common_period =	436	;	motionController.steps_for_decleration =	3426	;	break;
	case	297	:	motionController.actual_common_period =	435	;	motionController.steps_for_decleration =	3449	;	break;
	case	298	:	motionController.actual_common_period =	433	;	motionController.steps_for_decleration =	3473	;	break;
	case	299	:	motionController.actual_common_period =	432	;	motionController.steps_for_decleration =	3496	;	break;
	case	300	:	motionController.actual_common_period =	430	;	motionController.steps_for_decleration =	3519	;	break;
	case	301	:	motionController.actual_common_period =	429	;	motionController.steps_for_decleration =	3542	;	break;
	case	302	:	motionController.actual_common_period =	428	;	motionController.steps_for_decleration =	3566	;	break;
	case	303	:	motionController.actual_common_period =	426	;	motionController.steps_for_decleration =	3589	;	break;
	case	304	:	motionController.actual_common_period =	425	;	motionController.steps_for_decleration =	3613	;	break;
	case	305	:	motionController.actual_common_period =	423	;	motionController.steps_for_decleration =	3636	;	break;
	case	306	:	motionController.actual_common_period =	422	;	motionController.steps_for_decleration =	3660	;	break;
	case	307	:	motionController.actual_common_period =	421	;	motionController.steps_for_decleration =	3684	;	break;
	case	308	:	motionController.actual_common_period =	419	;	motionController.steps_for_decleration =	3708	;	break;
	case	309	:	motionController.actual_common_period =	418	;	motionController.steps_for_decleration =	3731	;	break;
	case	310	:	motionController.actual_common_period =	417	;	motionController.steps_for_decleration =	3755	;	break;
	case	311	:	motionController.actual_common_period =	415	;	motionController.steps_for_decleration =	3780	;	break;
	case	312	:	motionController.actual_common_period =	414	;	motionController.steps_for_decleration =	3804	;	break;
	case	313	:	motionController.actual_common_period =	413	;	motionController.steps_for_decleration =	3828	;	break;
	case	314	:	motionController.actual_common_period =	411	;	motionController.steps_for_decleration =	3852	;	break;
	case	315	:	motionController.actual_common_period =	410	;	motionController.steps_for_decleration =	3877	;	break;
	case	316	:	motionController.actual_common_period =	409	;	motionController.steps_for_decleration =	3901	;	break;
	case	317	:	motionController.actual_common_period =	408	;	motionController.steps_for_decleration =	3926	;	break;
	case	318	:	motionController.actual_common_period =	406	;	motionController.steps_for_decleration =	3950	;	break;
	case	319	:	motionController.actual_common_period =	405	;	motionController.steps_for_decleration =	3975	;	break;
	case	320	:	motionController.actual_common_period =	404	;	motionController.steps_for_decleration =	4000	;	break;
	case	321	:	motionController.actual_common_period =	402	;	motionController.steps_for_decleration =	4025	;	break;
	case	322	:	motionController.actual_common_period =	401	;	motionController.steps_for_decleration =	4049	;	break;
	case	323	:	motionController.actual_common_period =	400	;	motionController.steps_for_decleration =	4074	;	break;
	case	324	:	motionController.actual_common_period =	399	;	motionController.steps_for_decleration =	4100	;	break;
	case	325	:	motionController.actual_common_period =	398	;	motionController.steps_for_decleration =	4125	;	break;
	case	326	:	motionController.actual_common_period =	396	;	motionController.steps_for_decleration =	4150	;	break;
	case	327	:	motionController.actual_common_period =	395	;	motionController.steps_for_decleration =	4175	;	break;
	case	328	:	motionController.actual_common_period =	394	;	motionController.steps_for_decleration =	4201	;	break;
	case	329	:	motionController.actual_common_period =	393	;	motionController.steps_for_decleration =	4226	;	break;
	case	330	:	motionController.actual_common_period =	392	;	motionController.steps_for_decleration =	4252	;	break;
	}
}

/*
 * Calculates the differential between the two wheels and sets their individual period duration (speed)
 * */
void MOT_CalcualteDifferential(){
	uint16_t x = 100; // TODO: change the value x

	if (motionController.motorLeft.actual_period >= 0 && motionController.motorRight.actual_period >= 0){
		motionController.motorLeft.actual_period = motionController.actual_common_period + (x * motionController.differential);
		motionController.motorRight.actual_period = motionController.actual_common_period - (x * motionController.differential);
	}
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
			MOT_SetPeriod();

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

			MOT_SetSpeed();
			break;

		case MOT_FSM_RUN:

			MOT_CalcualteNOfSteps();

			LED_RED_Off();

			if((motionController.actual_common_period-motionController.target_common_period) > 500){
				motionController.state = MOT_FSM_ACCEL;
			}
			else if((motionController.target_common_period-motionController.actual_common_period) > 500){
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
			MOT_SetPeriod();

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

			MOT_SetSpeed();

			break;
	}
}

void vMotionControlTask(){
	for(;;){
		// have to be executed exactly every 10 ms
		if(motionController.running){
			MOT_Process();

			uint16_t i = motionController.actual_common_period;

			SER_SendUint16(i);
			SER_SendNewLine();


			RTOS_Wait(10);
		}
		else{
			// yield
		}
	}
}

/*
 * Initializes the MotionController. Different values could be defined here. e.g. minimal period time
 * */
void MOT_Init(void){
	motionController.running = FALSE;
	motionController.state = MOT_FSM_STOP;
	motionController.max_common_period = 65000;
	motionController.min_common_period = 200; 		// 0.2 ms minimale Periodendauer -> grösst mögliche Geschwindigkeit

	motionController.target_common_period = motionController.max_common_period;

	MOT_LEFT_Disable();
	MOT_RIGHT_Disable();

	RTOS_AddTask(vMotionControlTask, "MOT", 1);
}
