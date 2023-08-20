// C++ code
//
#include <string.h>

String valor;

void cross0_sequence(char status0[3], char status1[3], char status2[3], char status3[3], int time){
  
      //Como é um empurrador de bits, devemos começar pelo fim para que quem utiliza trabalhe de forma normal
    //Perceba que a sequencia 1100 será empurrada 1 -> 1-> -> 0 -> 0 o que resulta em 0011
  
  //cruzamento 3
  for(int j=0;j < 3 ;j++){
    if (status3[j] == '1'){
      digitalWrite(8, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(10, LOW);    	

    } else if (status3[j] == '0'){
      digitalWrite(8, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(10, LOW);    
    }
    digitalWrite(9, HIGH);
    digitalWrite(9, LOW);
  }
    
  //Cruzamento 2
  for(int j=0;j < 3 ;j++){
    if (status2[j] == '1'){
      digitalWrite(8, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(10, LOW);    	

    } else if (status2[j] == '0'){
      digitalWrite(8, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(10, LOW);    
    }
    digitalWrite(9, HIGH);
    digitalWrite(9, LOW);
  }
    
  //Cruzamento 1
  for(int j=0;j < 3 ;j++){
    if (status1[j] == '1'){
      digitalWrite(8, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(10, LOW);    	

    } else if (status1[j] == '0'){
      digitalWrite(8, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(10, LOW);    
    }
    digitalWrite(9, HIGH);
    digitalWrite(9, LOW);
  }
    
  //Cruzamento 0
  for(int j=0;j < 3 ;j++){
    if (status0[j] == '1'){
      digitalWrite(8, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(10, LOW);    	

    } else if (status0[j] == '0'){
      digitalWrite(8, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(10, LOW);    
    }
    digitalWrite(9, HIGH);
    digitalWrite(9, LOW);
  }
  
  digitalWrite(9, HIGH);
  digitalWrite(9, LOW);
	
  delay(time);
  
}

void default_sequence(int time){
  
  	//Adotamos para o amarelo apenas metade do tempo das outras luzes
  	//Isso porque na realidade o amarelo sempre dura menos
  
  	//-> 100 == RED -> 010 == YELLOW 001 -> GREEN +++ (sign 0, sign 1, sign 2, sign 3, status time)
	cross0_sequence("001", "100", "100", "100", time);
 	cross0_sequence("010", "100", "100", "100", (time/2));
  	cross0_sequence("100", "001", "100", "100", time);
  	cross0_sequence("100", "010", "100", "100", (time/2));
  	cross0_sequence("100", "100", "001", "100", time);
  	cross0_sequence("100", "100", "010", "100", (time/2));
  	cross0_sequence("100", "100", "100", "001", time);
  	cross0_sequence("100", "100", "100", "010", (time/2));
  
}
	

void setup()
{
  	pinMode(8, OUTPUT);
  	pinMode(9, OUTPUT);
  	pinMode(10, OUTPUT);
  	Serial.begin(9600);
}

void loop()
{
  		
  	//Essa parte pode ficar dentro de alguma função responsavel pelo sinal
  	//Perceba que podemos digitar os valores já em string para coordenar as luzes
  	//A API que gerenciará o tempo das luzes pode enviar o conjunto de dados
  	//da instrucao desejada e na proxima passada do serial ele usará o buffer
  	//Se não tiver nada no cache simplesmentes salta sem alteracao da sequencia de luzes
  	Serial.print("Digite algo: ");
  	valor = Serial.readString();
  	Serial.println(valor);
  	//Os valores jogados na porta serial fica como em cache
  	default_sequence(500);
   
}