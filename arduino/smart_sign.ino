#include <string.h>

void normal_sequence(char status0[3], char status1[3], char status2[3], char status3[3], int time){
  
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

void setup()
{
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop()
{
  
  //-> 100 == RED -> 010 == YELLOW 001 -> GREEN +++ (sign 0, sign 1, sign 2, sign 3, status time)
  normal_sequence("001", "100", "100", "100", 1000);
  normal_sequence("010", "100", "100", "100", 1000);
  normal_sequence("100", "001", "100", "100", 1000);
  normal_sequence("100", "010", "100", "100", 1000);
  normal_sequence("100", "100", "001", "100", 1000);
  normal_sequence("100", "100", "010", "100", 1000);
  normal_sequence("100", "100", "100", "001", 1000);
  normal_sequence("100", "100", "100", "010", 1000);
 
}