#include <stdio.h>
#include <stdlib.h>


typedef struct{
	int rule; /* Текущее правило 1: R, 2: L, 3: N(0), 4: U(180)*/
	int x;
	int y;
	
}Ant;

typedef struct{
	int color[20][3]; /* цвет для каждого правила - кол-во совпадает с count_rule и каждый цвет - 4 значения (цвет) */
	int rule[20]; /* Правила муравья 1: R, 2: L, 3: N(0), 4: U(180)*/
	int count_rule;	
	int px;
	int iteration;
}Field;


void input(Ant *inputed){
	int x, y, r;
	if (inputed == NULL){
		return;
	}
	printf("Введите координаты муравья и куда он смотрит (1 - право, 2 - лево, 3 - вверх, 4 - вниз)\n");
	/* Изначально вверх */
	while (scanf("%d%d%d", &(x), &(y), &(r)) != 3 
		    ||  r < 1 || r > 4){
		printf("Неверно\n");
		fflush(stdin);
	}
	inputed->x = x;
	inputed->y = y;
	inputed->rule = r;
}



void output(Ant *outputed){
	if (outputed == NULL){
		return;
	}
	printf("(%d, %d)\n", outputed->x, outputed->y);
	
	switch (outputed->rule){
		case 1:
			printf("Муравей смотрит вправо\n");
			break;
		case 2:
			printf("Муравей смотрит влево\n");
			break;
		case 3:
			printf("Муравей смотрит вверх\n");
			break;
		case 4:
			printf("Муравей смотрит вниз\n");
			break;
	}
}



void add_ant(Ant *a, int *n){
	if (*n > 39){
		printf("Превышен лимит муравьёв\n");
		return;
	}
	input(&a[*n]);
	(*n)++;
}


/* i начинается с 1*/
void del_ant(Ant *ants, int i, int *n){
	if (ants == NULL){
		return;
	}else if (*n == 0){
		printf("Нет муравьёв\n");
		return;
	}
	i -= 1;
	if (i < 0 || i > *n - 1){
		return;
	}
	Ant temp;
	while (i != *n - 1){
		temp = ants[i+1];
		ants[i+1] = ants[i];
		ants[i] = temp;
		i++;
	}
	(*n)--;
}
void input_rule(Field *field, int i){
	if (field){
		int rule, r, g, b;
		printf("Введите правило поведения муравья \n1: Право (-90)\n2: Лево (+90)\n3: Нейтрально(0)\n4: Обратно(180)\n");
		while (scanf("%d", &rule) != 1 || rule < 1 || rule > 4){
			printf("Неверно\n");
			fflush(stdin);	
		}
		(field->rule)[i] = rule;
		printf("Введите цвет для этого правила в формате rgb\n");
		while (scanf("%d%d%d", &r, &g, &b) != 3 || 
		r > 255 || g > 255 || b > 255 || 
		r < 0 || g < 0 || b < 0){
			printf("Неверно\n");
			fflush(stdin);	
		}	
		(field->color)[i][0] = r;
		(field->color)[i][1] = g;
		(field->color)[i][2] = b;
		
	}else{
		return;
	}

}

void input_field(Field *field){
	if (field){
		int i;
		
		printf("Введите размер пикселя\n");
		
		while (scanf("%d", &(field->px)) != 1){
			printf("Неверно\n");
			fflush(stdin);
		}
		
		printf("Введите количество правил (Максимум - 20)\n");
		
		while (scanf("%d", &(field->count_rule)) != 1 || field->count_rule > 20 || field->count_rule < 1){
			printf("Неверно\n");
			fflush(stdin);
		}
		
		
		
		for (i=0;i<field->count_rule;i++){
			input_rule(field, i);			
		}
		
		printf("Введите количество итераций\n");
		
		while(scanf("%d", &(field->iteration)) != 1){
			printf("Неверно\n");
			fflush(stdin);
		}
		
	}else{
		return;
	}
}

void change_rule(Field *field){
	int i;

	if (field && field->count_rule != 0){
		printf("Введите номер правила, которое хотите изменить\n");
		while (scanf("%d", &i) != 1 || field->count_rule < i || i < 1){
			printf("Неправильно\n");
			fflush(stdin);
		}
		i--;
		
		input_rule(field, i);
		
	}else{
		return;
	}
	
}

void save_file_field_main(Field *field){
	if (field){
		int m;
		FILE *f;
		f = fopen("files/field.dat", "wb");
		if (f){
			m = fwrite(field, sizeof(*field), 1, f);
			if (m != 0)
				printf("Успешное сохранение\n");
			else
				printf("Сохранение не удалось\n");
			fclose(f);
		}else
			printf("Сохранение не удалось\n");
	}else
		printf("Сохранение не удалось\n");
	
}



void save_file_ants_main(Ant *ants, int n){
	if (ants){
		int m;
		FILE *f;
		f = fopen("files/ants.dat", "wb");
		if (f){
			fwrite(&n, sizeof(n), 1, f);
			m = fwrite(ants, sizeof(*ants), n, f);
			if (m != 0)
				printf("Успешное сохранение\n");
			else
				printf("Сохранение не удалось\n");
			fclose(f);
		}else
			printf("Сохранение не удалось\n");
	}else
		printf("Сохранение не удалось\n");
	
}




void save_file_ants(Ant *ants,char *name, int n){
	FILE *f;
	if (ants){
		f = fopen(name, "wb");
		if (f){
			fwrite(&n, sizeof(n), 1, f);
			if (fwrite(ants, sizeof(*ants), n, f))
				printf("Успешное сохранение\n");
			else
				printf("Сохранение не удалось\n");
			fclose(f);
		}else
			printf("Сохранение не удалось\n");
	}else
		printf("Сохранение не удалось\n");
	
}
void save_file_field(Field *field, char *name){
	if (field && name){
		int m;
		FILE *f;
		f = fopen(name, "wb");
		if (f){
			m = fwrite(field, sizeof(*field), 1, f);
			if (m != 0)
				printf("Успешное сохранение\n");
			else
				printf("Сохранение не удалось\n");
			fclose(f);
		}else
			printf("Сохранение не удалось\n");
	}else
		printf("Сохранение не удалось\n");
	
}

void output_rule(int rule, int *color){
	if (color){
		switch (rule){
			case 1:
				printf("L(-90) ");
				break;
			case 2:
				printf("R(90) ");
				break;
			case 3:
				printf("N(0) ");
				break;
			case 4:
				printf("U(180) ");
				break;
		}
		printf("Цвет: (%d, %d, %d)\n", color[0], color[1], color[2]);
	}else{
		return;
	}
}

void output_field(Field *field){
	if (field && field->count_rule > 0){
		int i;
		printf("Размер пикселя - %d\n", field->px);
		printf("Количество правил: %d\n", field->count_rule);
		for (i=0;i < field->count_rule;i++){
			printf("%d Правило: ", i+1);
			output_rule((field->rule)[i], (field->color)[i]);
		}
		printf("Количество итераций = %d\n", field->iteration);
	}else{
		printf("Нет данных\n");
		return;
	}
}

void load_file_ants(Ant *ants, char *name, int *n){
	if (ants && name){
		FILE *f;
		f = fopen(name, "rb");
		if (f){
			fread(n, sizeof(*n), 1, f);
			fread(ants, sizeof(*ants), 40, f);
			if (*n != 0){
				printf("Успешная загрузка\n");
			}
			else
				printf("Загрузка не удалась\n");
			fclose(f);
		}else
			printf("Загрузка не удалась\n");
		
	}else
		printf("Загрузка не удалась\n");
}
void load_file_field(Field *field, char *name){
	if (field && name){
		FILE *f;
		int m;
		f = fopen(name, "rb");
		if (f){
			m = fread(field, sizeof(*field), 1, f);
			if (m != 0){
				printf("Успешная загрузка\n");
			}
			else
				printf("Загрузка не удалась\n");
			fclose(f);
		}else
			printf("Загрузка не удалась\n");
		
	}else
		printf("Загрузка не удалась\n");
}
int main(){
	Ant ants[40] = {0};
	Field field = {0};
	int n=0, i=0, menu=100;
	char name[100];
	system("chcp 1251 > nul");
	while (menu){	
			
		printf("Введите 1, чтобы добавить муравья (максимум - 40)\n");
		
		printf("Введите 2, чтобы удалить муравья\n");		
				
		printf("Введите 3, чтобы вывести муравьёв, обрабатываемых сейчас\n");
		
		printf("Введите 4, ввести параметры поля\n");		
				
		printf("Введите 5, чтобы изменить правило\n");
		
		printf("Введите 6, чтобы отчистить консоль\n");
		
		printf("Введите 7, сохранить параметры поля в главный файл\n");
		
		printf("Введите 8, сохранить муравьёв в главный файл\n");
		
		printf("Введите 9, чтобы вывести параметры поля\n");
		
		printf("Введите 10, сохранить параметры поля в сторонний файл (главный файл - files/field.dat)\n");
		
		printf("Введите 11, сохранить муравьёв в сторонний файл (главный файл - files/ants.dat)\n");
		
		printf("Введите 12, чтобы загрузить муравьёв из стороннего файла\n");
		
		printf("Введите 13, чтобы загрузить параметры поля из стороннего файла\n");
		
		printf("Введите -1 для выхода\n");
		
		while (scanf("%d", &menu) != 1){
			printf("Неправильный ввод\n");
			fflush(stdin);
		}
		
		switch(menu){
			case 1: /* Добавить муравья*/
				add_ant(ants, &n);
				break;
				
			case 2: /* Удалить элемент */
				printf("Каково муравья удалить?\n");
				while (scanf("%d", &i) != 1){
					printf("Неправильный ввод\n");
					fflush(stdin);
				}
				del_ant(ants, i, &n);
				break;
				
			case 3: /* Вывод */
				if (n == 0){
					printf("Нет Муравьёв\n");
				}
				for (i=0;i<n;i++){
					printf("Координаты %d Муравья: ", i + 1);
					output(&ants[i]);
				}

				break;
				
			case 4:
				input_field(&field);
				break;
				
			case 5:
				change_rule(&field);
				break;
				
			case 6:
				system("cls");
				break;
				
			case 7:
				save_file_field_main(&field);	
				break;		
			case 8:
				save_file_ants_main(ants, n);
				printf("Сохраненно %d муравьёв\n", n);
				break;
			
			case 9:
				output_field(&field);
				break;
			case 10:
				printf("Введите имя файла, в который хотим загрузить информацию информацию\n");
				while (scanf("%s", name) != 1){
					printf("Wrong input, try again\n");
				}
				printf("\n%s\n", name);
				save_file_field(&field, name);	
				break;
				
			case 11:
				printf("Введите имя файла, в который хотим загрузить информацию информацию\n");
				while (scanf("%s", name) != 1){
					printf("Wrong input, try again\n");
				}
				save_file_ants(ants, name, n);
				break;
			case 12:
				printf("Введите имя файла, из которого хотим загрузить информацию информацию\n");
				while (scanf("%s", name) != 1){
					printf("Wrong input, try again\n");
				}
				load_file_ants(ants, name, &n);
				break;
			case 13:
				printf("Введите имя файла, из которого хотим загрузить информацию информацию\n");
				while (scanf("%s", name) != 1){
					printf("Wrong input, try again\n");
				}
				load_file_field(&field, name);
				break;
			case 14:
				field.iteration = 1000000;
				break;
			case -1:
				printf("Спасибо, что воспользовались\n");
				system("pause");
				return 0;
			default:
				printf("Пока нет такой функции(\n");
				break;
				
			}	
		
	}
	system("pause");
	return 0;
} 


