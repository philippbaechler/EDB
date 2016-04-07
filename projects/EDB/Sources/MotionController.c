#include "PE_Types.h"
#include "MOT_LEFT.h"
#include "MOT_RIGHT.h"
#include "LED_RED.h"

#include "MotionController.h"
#include "Serial.h"


MOT_FSMData motionController;

/*
 * Initializes the MotionController. Different values could be defined here. e.g. minimal period time
 * */
void MOT_Init(void){
	motionController.running = FALSE;
	motionController.state = MOT_FSM_STOP;
	motionController.max_common_period = 65000;
	motionController.min_common_period = 200; 		// 0.2 ms minimale Periodendauer -> grösst mogliche Geschwindigkeit

	MOT_LEFT_Disable();
}

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
uint16_t MOT_GetSpeed(){

	uint16 r;

	switch (motionController.accleration_counter){
	case	0	:	r = 	65000	;	break;
	case	1	:	r = 	43333	;	break;
	case	2	:	r = 	32500	;	break;
	case	3	:	r = 	26000	;	break;
	case	4	:	r = 	21667	;	break;
	case	5	:	r = 	18571	;	break;
	case	6	:	r = 	16250	;	break;
	case	7	:	r = 	14444	;	break;
	case	8	:	r = 	13000	;	break;
	case	9	:	r = 	11818	;	break;
	case	10	:	r = 	10833	;	break;
	case	11	:	r = 	10000	;	break;
	case	12	:	r = 	9286	;	break;
	case	13	:	r = 	8667	;	break;
	case	14	:	r = 	8125	;	break;
	case	15	:	r = 	7647	;	break;
	case	16	:	r = 	7222	;	break;
	case	17	:	r = 	6842	;	break;
	case	18	:	r = 	6500	;	break;
	case	19	:	r = 	6190	;	break;
	case	20	:	r = 	5909	;	break;
	case	21	:	r = 	5652	;	break;
	case	22	:	r = 	5417	;	break;
	case	23	:	r = 	5200	;	break;
	case	24	:	r = 	5000	;	break;
	case	25	:	r = 	4815	;	break;
	case	26	:	r = 	4643	;	break;
	case	27	:	r = 	4483	;	break;
	case	28	:	r = 	4333	;	break;
	case	29	:	r = 	4194	;	break;
	case	30	:	r = 	4063	;	break;
	case	31	:	r = 	3939	;	break;
	case	32	:	r = 	3824	;	break;
	case	33	:	r = 	3714	;	break;
	case	34	:	r = 	3611	;	break;
	case	35	:	r = 	3514	;	break;
	case	36	:	r = 	3421	;	break;
	case	37	:	r = 	3333	;	break;
	case	38	:	r = 	3250	;	break;
	case	39	:	r = 	3171	;	break;
	case	40	:	r = 	3095	;	break;
	case	41	:	r = 	3023	;	break;
	case	42	:	r = 	2955	;	break;
	case	43	:	r = 	2889	;	break;
	case	44	:	r = 	2826	;	break;
	case	45	:	r = 	2766	;	break;
	case	46	:	r = 	2708	;	break;
	case	47	:	r = 	2653	;	break;
	case	48	:	r = 	2600	;	break;
	case	49	:	r = 	2549	;	break;
	case	50	:	r = 	2500	;	break;
	case	51	:	r = 	2453	;	break;
	case	52	:	r = 	2407	;	break;
	case	53	:	r = 	2364	;	break;
	case	54	:	r = 	2321	;	break;
	case	55	:	r = 	2281	;	break;
	case	56	:	r = 	2241	;	break;
	case	57	:	r = 	2203	;	break;
	case	58	:	r = 	2167	;	break;
	case	59	:	r = 	2131	;	break;
	case	60	:	r = 	2097	;	break;
	case	61	:	r = 	2063	;	break;
	case	62	:	r = 	2031	;	break;
	case	63	:	r = 	2000	;	break;
	case	64	:	r = 	1970	;	break;
	case	65	:	r = 	1940	;	break;
	case	66	:	r = 	1912	;	break;
	case	67	:	r = 	1884	;	break;
	case	68	:	r = 	1857	;	break;
	case	69	:	r = 	1831	;	break;
	case	70	:	r = 	1806	;	break;
	case	71	:	r = 	1781	;	break;
	case	72	:	r = 	1757	;	break;
	case	73	:	r = 	1733	;	break;
	case	74	:	r = 	1711	;	break;
	case	75	:	r = 	1688	;	break;
	case	76	:	r = 	1667	;	break;
	case	77	:	r = 	1646	;	break;
	case	78	:	r = 	1625	;	break;
	case	79	:	r = 	1605	;	break;
	case	80	:	r = 	1585	;	break;
	case	81	:	r = 	1566	;	break;
	case	82	:	r = 	1548	;	break;
	case	83	:	r = 	1529	;	break;
	case	84	:	r = 	1512	;	break;
	case	85	:	r = 	1494	;	break;
	case	86	:	r = 	1477	;	break;
	case	87	:	r = 	1461	;	break;
	case	88	:	r = 	1444	;	break;
	case	89	:	r = 	1429	;	break;
	case	90	:	r = 	1413	;	break;
	case	91	:	r = 	1398	;	break;
	case	92	:	r = 	1383	;	break;
	case	93	:	r = 	1368	;	break;
	case	94	:	r = 	1354	;	break;
	case	95	:	r = 	1340	;	break;
	case	96	:	r = 	1327	;	break;
	case	97	:	r = 	1313	;	break;
	case	98	:	r = 	1300	;	break;
	case	99	:	r = 	1287	;	break;
	case	100	:	r = 	1275	;	break;
	case	101	:	r = 	1262	;	break;
	case	102	:	r = 	1250	;	break;
	case	103	:	r = 	1238	;	break;
	case	104	:	r = 	1226	;	break;
	case	105	:	r = 	1215	;	break;
	case	106	:	r = 	1204	;	break;
	case	107	:	r = 	1193	;	break;
	case	108	:	r = 	1182	;	break;
	case	109	:	r = 	1171	;	break;
	case	110	:	r = 	1161	;	break;
	case	111	:	r = 	1150	;	break;
	case	112	:	r = 	1140	;	break;
	case	113	:	r = 	1130	;	break;
	case	114	:	r = 	1121	;	break;
	case	115	:	r = 	1111	;	break;
	case	116	:	r = 	1102	;	break;
	case	117	:	r = 	1092	;	break;
	case	118	:	r = 	1083	;	break;
	case	119	:	r = 	1074	;	break;
	case	120	:	r = 	1066	;	break;
	case	121	:	r = 	1057	;	break;
	case	122	:	r = 	1048	;	break;
	case	123	:	r = 	1040	;	break;
	case	124	:	r = 	1032	;	break;
	case	125	:	r = 	1024	;	break;
	case	126	:	r = 	1016	;	break;
	case	127	:	r = 	1008	;	break;
	case	128	:	r = 	1000	;	break;
	case	129	:	r = 	992	;	break;
	case	130	:	r = 	985	;	break;
	case	131	:	r = 	977	;	break;
	case	132	:	r = 	970	;	break;
	case	133	:	r = 	963	;	break;
	case	134	:	r = 	956	;	break;
	case	135	:	r = 	949	;	break;
	case	136	:	r = 	942	;	break;
	case	137	:	r = 	935	;	break;
	case	138	:	r = 	929	;	break;
	case	139	:	r = 	922	;	break;
	case	140	:	r = 	915	;	break;
	case	141	:	r = 	909	;	break;
	case	142	:	r = 	903	;	break;
	case	143	:	r = 	897	;	break;
	case	144	:	r = 	890	;	break;
	case	145	:	r = 	884	;	break;
	case	146	:	r = 	878	;	break;
	case	147	:	r = 	872	;	break;
	case	148	:	r = 	867	;	break;
	case	149	:	r = 	861	;	break;
	case	150	:	r = 	855	;	break;
	case	151	:	r = 	850	;	break;
	case	152	:	r = 	844	;	break;
	case	153	:	r = 	839	;	break;
	case	154	:	r = 	833	;	break;
	case	155	:	r = 	828	;	break;
	case	156	:	r = 	823	;	break;
	case	157	:	r = 	818	;	break;
	case	158	:	r = 	813	;	break;
	case	159	:	r = 	807	;	break;
	case	160	:	r = 	802	;	break;
	case	161	:	r = 	798	;	break;
	case	162	:	r = 	793	;	break;
	case	163	:	r = 	788	;	break;
	case	164	:	r = 	783	;	break;
	case	165	:	r = 	778	;	break;
	case	166	:	r = 	774	;	break;
	case	167	:	r = 	769	;	break;
	case	168	:	r = 	765	;	break;
	case	169	:	r = 	760	;	break;
	case	170	:	r = 	756	;	break;
	case	171	:	r = 	751	;	break;
	case	172	:	r = 	747	;	break;
	case	173	:	r = 	743	;	break;
	case	174	:	r = 	739	;	break;
	case	175	:	r = 	734	;	break;
	case	176	:	r = 	730	;	break;
	case	177	:	r = 	726	;	break;
	case	178	:	r = 	722	;	break;
	case	179	:	r = 	718	;	break;
	case	180	:	r = 	714	;	break;
	case	181	:	r = 	710	;	break;
	case	182	:	r = 	707	;	break;
	case	183	:	r = 	703	;	break;
	case	184	:	r = 	699	;	break;
	case	185	:	r = 	695	;	break;
	case	186	:	r = 	691	;	break;
	case	187	:	r = 	688	;	break;
	case	188	:	r = 	684	;	break;
	case	189	:	r = 	681	;	break;
	case	190	:	r = 	677	;	break;
	case	191	:	r = 	674	;	break;
	case	192	:	r = 	670	;	break;
	case	193	:	r = 	667	;	break;
	case	194	:	r = 	663	;	break;
	case	195	:	r = 	660	;	break;
	case	196	:	r = 	657	;	break;
	case	197	:	r = 	653	;	break;
	case	198	:	r = 	650	;	break;
	case	199	:	r = 	647	;	break;
	case	200	:	r = 	644	;	break;
	case	201	:	r = 	640	;	break;
	case	202	:	r = 	637	;	break;
	case	203	:	r = 	634	;	break;
	case	204	:	r = 	631	;	break;
	case	205	:	r = 	628	;	break;
	case	206	:	r = 	625	;	break;
	case	207	:	r = 	622	;	break;
	case	208	:	r = 	619	;	break;
	case	209	:	r = 	616	;	break;
	case	210	:	r = 	613	;	break;
	case	211	:	r = 	610	;	break;
	case	212	:	r = 	607	;	break;
	case	213	:	r = 	605	;	break;
	case	214	:	r = 	602	;	break;
	case	215	:	r = 	599	;	break;
	case	216	:	r = 	596	;	break;
	case	217	:	r = 	594	;	break;
	case	218	:	r = 	591	;	break;
	case	219	:	r = 	588	;	break;
	case	220	:	r = 	586	;	break;
	case	221	:	r = 	583	;	break;
	case	222	:	r = 	580	;	break;
	case	223	:	r = 	578	;	break;
	case	224	:	r = 	575	;	break;
	case	225	:	r = 	573	;	break;
	case	226	:	r = 	570	;	break;
	case	227	:	r = 	568	;	break;
	case	228	:	r = 	565	;	break;
	case	229	:	r = 	563	;	break;
	case	230	:	r = 	560	;	break;
	case	231	:	r = 	558	;	break;
	case	232	:	r = 	556	;	break;
	case	233	:	r = 	553	;	break;
	case	234	:	r = 	551	;	break;
	case	235	:	r = 	549	;	break;
	case	236	:	r = 	546	;	break;
	case	237	:	r = 	544	;	break;
	case	238	:	r = 	542	;	break;
	case	239	:	r = 	539	;	break;
	case	240	:	r = 	537	;	break;
	case	241	:	r = 	535	;	break;
	case	242	:	r = 	533	;	break;
	case	243	:	r = 	531	;	break;
	case	244	:	r = 	528	;	break;
	case	245	:	r = 	526	;	break;
	case	246	:	r = 	524	;	break;
	case	247	:	r = 	522	;	break;
	case	248	:	r = 	520	;	break;
	case	249	:	r = 	518	;	break;
	case	250	:	r = 	516	;	break;
	case	251	:	r = 	514	;	break;
	case	252	:	r = 	512	;	break;
	case	253	:	r = 	510	;	break;
	case	254	:	r = 	508	;	break;
	case	255	:	r = 	506	;	break;
	case	256	:	r = 	504	;	break;
	case	257	:	r = 	502	;	break;
	case	258	:	r = 	500	;	break;
	case	259	:	r = 	498	;	break;
	case	260	:	r = 	496	;	break;
	case	261	:	r = 	494	;	break;
	case	262	:	r = 	492	;	break;
	case	263	:	r = 	491	;	break;
	case	264	:	r = 	489	;	break;
	case	265	:	r = 	487	;	break;
	case	266	:	r = 	485	;	break;
	case	267	:	r = 	483	;	break;
	case	268	:	r = 	481	;	break;
	case	269	:	r = 	480	;	break;
	case	270	:	r = 	478	;	break;
	case	271	:	r = 	476	;	break;
	case	272	:	r = 	474	;	break;
	case	273	:	r = 	473	;	break;
	case	274	:	r = 	471	;	break;
	case	275	:	r = 	469	;	break;
	case	276	:	r = 	468	;	break;
	case	277	:	r = 	466	;	break;
	case	278	:	r = 	464	;	break;
	case	279	:	r = 	463	;	break;
	case	280	:	r = 	461	;	break;
	case	281	:	r = 	459	;	break;
	case	282	:	r = 	458	;	break;
	case	283	:	r = 	456	;	break;
	case	284	:	r = 	455	;	break;
	case	285	:	r = 	453	;	break;
	case	286	:	r = 	451	;	break;
	case	287	:	r = 	450	;	break;
	case	288	:	r = 	448	;	break;
	case	289	:	r = 	447	;	break;
	case	290	:	r = 	445	;	break;
	case	291	:	r = 	444	;	break;
	case	292	:	r = 	442	;	break;
	case	293	:	r = 	441	;	break;
	case	294	:	r = 	439	;	break;
	case	295	:	r = 	438	;	break;
	case	296	:	r = 	436	;	break;
	case	297	:	r = 	435	;	break;
	case	298	:	r = 	433	;	break;
	case	299	:	r = 	432	;	break;
	case	300	:	r = 	430	;	break;
	case	301	:	r = 	429	;	break;
	case	302	:	r = 	428	;	break;
	case	303	:	r = 	426	;	break;
	case	304	:	r = 	425	;	break;
	case	305	:	r = 	423	;	break;
	case	306	:	r = 	422	;	break;
	case	307	:	r = 	421	;	break;
	case	308	:	r = 	419	;	break;
	case	309	:	r = 	418	;	break;
	case	310	:	r = 	417	;	break;
	case	311	:	r = 	415	;	break;
	case	312	:	r = 	414	;	break;
	case	313	:	r = 	413	;	break;
	case	314	:	r = 	411	;	break;
	case	315	:	r = 	410	;	break;
	case	316	:	r = 	409	;	break;
	case	317	:	r = 	408	;	break;
	case	318	:	r = 	406	;	break;
	case	319	:	r = 	405	;	break;
	case	320	:	r = 	404	;	break;
	case	321	:	r = 	402	;	break;
	case	322	:	r = 	401	;	break;
	case	323	:	r = 	400	;	break;
	case	324	:	r = 	399	;	break;
	case	325	:	r = 	398	;	break;
	case	326	:	r = 	396	;	break;
	case	327	:	r = 	395	;	break;
	case	328	:	r = 	394	;	break;
	case	329	:	r = 	393	;	break;
	case	330	:	r = 	392	;	break;
	}

	return r;
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

	if (motionController.steps_left_until_stop > motionController.steps_for_decleration){
		motionController.steps_left_until_stop - newSteps;
	}
}

void MOT_Process(){

	switch(motionController.state){
		case MOT_FSM_STOP:

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
			motionController.actual_common_period = MOT_GetSpeed();

			if(motionController.actual_common_period <= motionController.target_common_period){
				motionController.state = MOT_FSM_RUN;
				LED_RED_On();
			}

			/* Check if we have to start the deceleration for a stop */
			if(motionController.steps_for_decleration <= motionController.steps_left_until_stop){
				motionController.target_common_period = motionController.max_common_period;
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
			if(motionController.steps_for_decleration <= motionController.steps_left_until_stop){
				motionController.target_common_period = motionController.max_common_period;
				motionController.state = MOT_FSM_DECEL;
			}

			break;

		case MOT_FSM_DECEL:

			MOT_CalcualteNOfSteps();

			motionController.accleration_counter--;
			motionController.actual_common_period = MOT_GetSpeed();

			if(motionController.actual_common_period >= motionController.target_common_period){

				if (motionController.target_common_period == motionController.max_common_period){

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
				motionController.state = MOT_FSM_STOP;
				MOT_LEFT_Disable();
				MOT_RIGHT_Disable();
			}

			MOT_SetSpeed();

			break;
	}
}
