/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Kyeongchul Kim <kyeongchul.kim@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sim_common.h>

sim_info_t g_si;

int sim_pin_info_setup()
{
	FILE * p_file;
	int f_size;
	char* p_buf;
	char* p_str = NULL;
	size_t result;
		
	p_file = fopen("sim_pw.txt","r");
	if(p_file == NULL)
		printf("file not found\n");
	
	fseek(p_file, 0, SEEK_END);
	f_size = ftell(p_file);
	rewind(p_file);
	
	//printf("file size[%d]\n",f_size);
		
	p_buf = (char*)malloc(sizeof(char)*f_size);
	if(p_buf == NULL)
		printf("file is empty or malloc failed\n");
	
	result = fread(p_buf,1,f_size,p_file);
	if(result != f_size)
		printf("file read error\n");
	
	memset(&p_buf[f_size-1],'\0',1);
		
	//	printf("file info=%s\n",p_buf);
	
	p_str = strstr(p_buf, "pin1:");  
	if(p_str)sscanf(p_str+5, "%4s", &g_si.pin1[0]);	
		
	p_str = strstr(p_buf, "pin2:");  
	if(p_str)sscanf(p_str+5, "%4s", &g_si.pin2[0]);	
	
	p_str = strstr(p_buf, "puk1:");  
	if(p_str)sscanf(p_str+5, "%8s", &g_si.puk1[0]);	
	
	p_str = strstr(p_buf, "puk2:");  
	if(p_str)sscanf(p_str+5, "%8s", &g_si.puk2[0]);	
	
	p_str = strstr(p_buf, "wpin1:");  
	if(p_str)sscanf(p_str+6, "%4s", &g_si.wpin1[0]); 
	
	p_str = strstr(p_buf, "wpin2:");  
	if(p_str)sscanf(p_str+6, "%4s", &g_si.wpin2[0]); 
	
	p_str = strstr(p_buf, "wpuk1:");  
	if(p_str)sscanf(p_str+6, "%8s", &g_si.wpuk1[0]); 
	
	p_str = strstr(p_buf, "wpuk2:");  
	if(p_str)sscanf(p_str+6, "%8s", &g_si.wpuk2[0]); 
	
	g_si.pin1[4]='\0';
	g_si.pin2[4]='\0';
	g_si.puk1[8]='\0';
	g_si.puk2[8]='\0';
	g_si.wpin1[4]='\0';
	g_si.wpin2[4]='\0';
	g_si.wpuk1[8]='\0';
	g_si.wpuk2[8]='\0';
		
	printf("pin1[%s]\n",g_si.pin1);
	printf("pin2[%s]\n",g_si.pin2);
	printf("puk1[%s]\n",g_si.puk1);
	printf("puk2[%s]\n",g_si.puk2);
	printf("wpin1[%s]\n",g_si.wpin1);
	printf("wpin2[%s]\n",g_si.wpin2);
	printf("wpuk1[%s]\n",g_si.wpuk1);
	printf("wpuk2[%s]\n",g_si.wpuk2);
	
	fclose(p_file);
	free(p_buf);
	return 0;	
}

