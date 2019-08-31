#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <json/json.h>
#include <string.h>
#include <time.h>

int get_json_int_filed(const char *json_data,const char *filed){
	int filed_value;
	json_object *jobject;
	jobject = json_tokener_parse(json_data);
	if(jobject == NULL){
		printf("Json data format error!\n");
		return -1;
	}
	struct json_object *filed_object = json_object_object_get(jobject,filed);
	if(filed_object == NULL){
		json_object_put(jobject);
		printf("Json object is not exit %s filed\n",filed);
		return -1;
	}
	filed_value = json_object_get_int(filed_object);
	json_object_put(filed_object);
	json_object_put(jobject);

	return filed_value;
}

char *get_json_string_filed(const char *json_data,const char *filed){
	char *str;
	const char *filed_value;
	json_object *jobject;
	jobject = json_tokener_parse(json_data);
	if(jobject == NULL){
		printf("Json data format error!\n");
		return NULL;
	}
	struct json_object *filed_object = json_object_object_get(jobject,filed);
	if(filed_object == NULL){
		json_object_put(jobject);
		printf("Json object is not exit %s filed\n",filed);
		return NULL;
	}
	filed_value = json_object_get_string(filed_object);
	str = strdup(filed_value);
	json_object_put(filed_object);
	json_object_put(jobject);

	return str;
}

void ack_client_register(int sockfd,int exist_flag){
	const char *json_data;
	char *error_string;
	json_object *jobject;

	jobject = json_object_new_object();
	json_object_object_add(jobject,"functionId",json_object_new_int(1));
	json_object_object_add(jobject,"statu",json_object_new_int(exist_flag));
	if(exist_flag){
		error_string = "the user is exist";
	}
	else{
		error_string = "";
	}
	json_object_object_add(jobject,"error",json_object_new_string(error_string));
	json_data = json_object_to_json_string(jobject);
	write(sockfd,json_data,strlen(json_data));
	json_object_put(jobject);

	return;
}

void ack_client_login(int sockfd,int exist_flag){
	const char *json_data;
	char *error_string;
	json_object *jobject;

	jobject = json_object_new_object();
	json_object_object_add(jobject,"functionId",json_object_new_int(2));
	json_object_object_add(jobject,"statu",json_object_new_int(!exist_flag));

	if(exist_flag){
		char clientId[100];
		error_string = "";
		sprintf(clientId,"%d",getppid());
		json_object_object_add(jobject,"clientId",json_object_new_string(clientId));
	}
	else{
		error_string = "The username or password error";
	}

	json_object_object_add(jobject,"error",json_object_new_string(error_string));
	json_data = json_object_to_json_string(jobject);

	write(sockfd,json_data,strlen(json_data));
	json_object_put(jobject);

	return;
}

void client_register(int sockfd,const char *json_data){	
	FILE *fp;
	int exist_flag = 0;
	char *username = get_json_string_filed(json_data,"username");
	char *password = get_json_string_filed(json_data,"password");
	char *useremail = get_json_string_filed(json_data,"email");

	fp = fopen("user.txt","a+");
	if(fp == NULL){
		perror("Fail to fopen");
		return;
	}

	while(1){
		char _username[100];
		char _password[100];
		char _useremail[100];
		printf("---------information----------\n");
		int ret = fscanf(fp,"%[^:]:%[^:]:%s\n",_username,_password,_useremail);
		printf("username:%s\n",_username);
		printf("password:%s\n",_password);
		if(ret == EOF){
			break;
		}
		if(strcmp(username,_username) == 0){
			exist_flag = 1;
			goto next;
		}
	}

	fprintf(fp,"%s:%s:%s\n",username,password,useremail);

next:
	fclose(fp);
	free(username);
	free(password);
	free(useremail);
	
	ack_client_register(sockfd,exist_flag);

	return;
}

/*void client_login(int sockfd,const char *json_data ){
	printf("-0\n");
	FILE *fp;
	int exist_flag = 0;
	char *username = get_json_string_filed(json_data,"username");
	char *password = get_json_string_filed(json_data,"password");

	fp = fopen("user.txt","a+");
	if(fp = NULL){
		perror("Fail to fopen");
		return;
	}
	printf("-1\n");

	while(1){
		printf("-2\n");
		char _username[100];
		printf("-12\n");
		char _password[100];
		printf("-13\n");
		char _email[100];
	//	printf("-11");
		int ret = fscanf(fp,"%[^:]:%[^:]:%s\n",_username,_password,_email);
		printf("-3\n");
		if(ret == EOF){
			printf("-4\n");
			break;
		}
	printf("-5\n");
		if(strcmp(username,_username) == 0){
			exist_flag = 2;
			printf("-6\n");
			goto next;
		}
	printf("-7\n");
	}
next:
	printf("-8\n");
	fclose(fp);
	free(username);
	free(password);
	return;
}*/

void client_login(int sockfd,const char *json_data){
	FILE *fp;
	int exist_flag = 0;
	char *username = get_json_string_filed(json_data,"username");
	char *password = get_json_string_filed(json_data,"password");
	
	fp = fopen("user.txt","r");
	if(fp == NULL){
		perror("Fail to fopen");
	}

	while(1){
		char _username[100];
		char _password[100];
		char _useremail[100];

		int ret = fscanf(fp,"%[^:]:%[^:]:%s\n",_username,_password,_useremail);
		if(ret == EOF){
			break;
		}

		if(strcmp(username,_username) == 0 && strcmp(password,_password) == 0){
			exist_flag = 1;
			goto next;
		}
	}
next:
	fclose(fp);
	free(username);
	free(password);
	ack_client_login(sockfd,exist_flag);
	return;
}

void control_led_1(int control_statu){
	FILE *fp;
	fp = fopen("/sys/class/leds/fs4412-led1/brightness","w");

	if(control_statu == 0){
		fputs("1",fp);
	}
	else{
		fputs("0",fp);
	}
	fclose(fp);
	return;
}

void control_led_2(int control_statu){
	FILE *fp;
	fp = fopen("/sys/class/leds/fs4412-led2/brightness","w");

	if(control_statu == 0){
		fputs("1",fp);
	}
	else{
		fputs("0",fp);
	}
	fclose(fp);
	return;
}

void control_fan(int control_statu){
	if(control_statu == 0){
		printf("open FAN\n");
	}
	else{
		printf("close FAN\n");
	}

	return;
}

void control_door(int control_statu){
	FILE *fp;
	fp = fopen("/sys/class/backlight/beep-backlight.6/brightness","w");

	if(control_statu == 0){
		fputs("1",fp);
	}
	else{
		fputs("0",fp);
	}
	fclose(fp);
	return;
}

client_device_control(int sockfd,const char *json_data){
	enum{
		LED_1 = 1,
		LED_2 = 2,
		FAN = 3,
		DOOR = 4,
	};
	int deviceId;
	int control_statu;
	char *clientId = get_json_string_filed(json_data,"clientId");
	printf("clientId:%s\n",clientId);
	if(clientId == NULL){
		return;
	}
	if(getppid() != atoi(clientId)){
		free(clientId);
		return;
	}

	control_statu = get_json_int_filed(json_data,"controlStatu");
	deviceId = get_json_int_filed(json_data,"deviceId");

	switch(deviceId){
	case LED_1:
		control_led_1(control_statu);
		break;

	case LED_2:
		control_led_2(control_statu);
		break;

	case FAN:
		control_fan(control_statu);
		break;

	case DOOR:
		control_door(control_statu);
		break;
	}

	free(clientId);

	return;
}

void ack_client_auth(int sockfd,const char *json_data){
	char *revAuth;
	json_object *jobject;
	char *error;
	char *auth;
	FILE *fp;
	int statu;
	char *name = get_json_string_filed(json_data,"username");
	char *email = get_json_string_filed(json_data,"email");
	char *password = get_json_string_filed(json_data,"password");
	
	printf("name:%s",name);

	jobject = json_object_new_object();
	revAuth = get_json_string_filed(json_data,"authcord");
	fp = fopen("auth.txt","r");
	fgets(auth,5,fp);
	fclose(fp);

	if(strcmp(revAuth,auth) == 0){
		statu = 1;
		error = "";
		FILE *fr = fopen("user.txt","a+");
		while(1){
			char _name[100];
			char _email[100];
			char _password[100];
			int h;
			
			fscanf(fp,"%[^:]:%[^:]:%s\n",_name,_password,_email);
			h = ftell(fr);
			if(strcmp(name,_name) == 0,strcmp(email,_email)){
				fseek(fr,h-1,SEEK_SET);
				fprintf(fr,"%s:%s:%s\n",name,password,email);
				break;
			}
		}
		fclose(fr);
	}
	else{
		statu = 0;
		error = "the auth cord error";
	}
	

	json_object_object_add(jobject,"functionId",json_object_new_int(6));
	json_object_object_add(jobject,"error",json_object_new_string(error));
	json_object_object_add(jobject,"statu",json_object_new_int(!statu));
	json_data = json_object_to_json_string(jobject);
	write(sockfd,json_data,strlen(json_data));
	return;
	
}

int name_macth_email(char *name,char *email){
	int ok;
	char _name[100];
	char _password[100];
	char _email[100];
	FILE *fp = fopen("user.txt","r");
	while(1){
	fscanf(fp,"%[^:]:%[^:]:%s\n",_name,_password,_email);
	if(strcmp(name,_name) == 0,strcmp(email,_email) == 0){
		printf("%s\n",email);
		ok = 1;
		break;
	}
	else{
		ok = 0;
	}
	}
	fclose(fp);
	
	return ok;
}

void forgetPw(int sockfd,const char *json_data){
	char *name = get_json_string_filed(json_data,"username");
	char *email = get_json_string_filed(json_data,"email");
	printf("name:%s\n",name);
	char _email[100];
	FILE *fp;
	FILE *fc;
	srand(time(NULL));
	int auth = rand() % 10000;
	json_object *jobject;

	jobject = json_object_new_object();
	json_object_object_add(jobject,"functionId",json_object_new_int(4));
	if(name_macth_email(name,email)){
		fp = fopen("echo.txt","w");
		if(fp == NULL){
			perror("Fail to fopen echo.txt\n");
			return;
		}

		fprintf(fp,"echo \" %d\" | mutt -s \"重置密码\" %s\n",auth,email);
		fclose(fp);

		FILE *fr;
		fr = fopen("echo.txt","r");
		if(fr == NULL){
			perror("Fail to fopen echo.txt\n");
		}
		fgets(_email,100,fr);
		fclose(fr);
		free(email);

		FILE *fa;
		fa = fopen("auth.txt","w");
		if(fa == NULL){
			perror("Fail to fopen auth.txt\n");
			return;
		}
		fprintf(fa,"%d",auth);

		fclose(fa);

		write(sockfd,json_data,strlen(json_data));
		system(_email);
	}

	return;
}

void process_task(int sockfd,const char *json_data){
	enum{
		REGISTER = 1,
		LOGIN = 2,
		DEVICE_CONTROL = 3,
		FORGETPW = 4,
		RESET = 6,
	};
	int functionId;

	functionId = get_json_int_filed(json_data,"functionId");
	if(functionId < 0){
		return;
	}
	switch(functionId){
		case REGISTER:
			client_register(sockfd,json_data);
			break;
		case LOGIN:
			client_login(sockfd,json_data);
				printf("--1");
			break;
		case DEVICE_CONTROL:
			client_device_control(sockfd,json_data);
			break;
		case FORGETPW:
			forgetPw(sockfd,json_data);
			break;
		case RESET:
			ack_client_auth(sockfd,json_data);
			break;
	}
}

void do_client(int clientfd){
	int ret;
	char buf[1024];
	while(1){
		printf("-----------read-----------\n");
		ret = read(clientfd,buf,sizeof(buf) - 1);
		if(ret <= 0){
			break;
		}
		buf[ret] = '\0';
		printf("Read %d bytes:%s\n",ret,buf);
		process_task(clientfd,buf);
	}
	exit(0);
}

int main(int argc,const char *argv[]){
	int listenfd;
	int ret;
	int clientfd;
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
	int addrlen = sizeof(clientfd);
	char buf[1024];
	pid_t pid;
	

//	openMedia();


	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd < 0){
		perror("Fail to create socket");
		return -1;
	}
	printf("socket id : %d\n",listenfd);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8089);
	//server_addr.sin_addr.s_addr = inet_addr("192.168.40.128");
	server_addr.sin_addr.s_addr = 0;
	ret = bind(listenfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(ret < 0){
		perror("Fail to bind");
		return -1;
	}
	while(1){
		ret = listen(listenfd,128);
		if(ret < 0){
			perror("Fail to listen");
			return -1;
		}
		printf("listen...\n");
		clientfd = accept(listenfd,(struct sockaddr *)&client_addr,&addrlen);
		if(clientfd < 0){
			perror("Fail to accept");
			return -1;

		}
		printf("-------------accept--------------\n");
		printf("cientfd:%d\n",clientfd);
		printf("client ip:%s\n",inet_ntoa(client_addr.sin_addr));
		printf("client port:%d\n",ntohs(client_addr.sin_port));

		pid = fork();
		if(pid < 0){
			perror("Fail to fork");
			return -1;
		}
		if(pid == 0){
			close(listenfd);
			do_client(clientfd);
		}
		close(clientfd);
	}
	return 0;
}
