import java.lang.Character;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;


public class front{
    public static int charClass;
    public static String lexeme = "";
    public static char nextChar;
    public static int lexLen;
    public static int token;
    public static int nextToken;
    public static int ch;
    public static FileReader fileReader = null;
   
    static final int EOF = -1; //EOF에 대한 토큰
    static final int LETTER = 0; //문자인지 확인하기 위한 토큰
    static final int DIGIT = 1; //숫자인지 확인하기 위한 토큰
    static final int UNKNOWN = 99; //미확인 문자에 대한 토큰
    
    static final int INT_LIT = 10; //해당 값을 상수로 판별
    static final int IDENT = 11; //identifier 토큰
    static final int ASSIGN_OP = 20; // "=" 토큰
    static final int ADD_OP = 21; // "+" 토큰
    static final int SUB_OP = 22; // "-" 토큰
    static final int MULT_OP = 23; // "*" 토큰
    static final int DIV_OP = 24; // "/" 토큰
    static final int LEFT_PAREN = 25; // "(" 토큰
    static final int RIGHT_PAREN = 26; // ")" 토큰
    
    static final int FOR_CODE = 30; // for 토큰
    static final int IF_CODE = 31; // if 토큰
    static final int ELSE_CODE = 32; // else 토큰
    static final int WHILE_CODE = 33; // while 토큰
    static final int DO_CODE = 34; // do 토큰
    static final int INT_CODE = 35; //int 토큰
    static final int FLOAT_CODE = 36; //float 토큰
    static final int SWITCH_CODE = 37; //switch 토큰
    
        

	public static void main(String[] args){

       

        try{
            
            fileReader = new FileReader(new File("front.in")); //front.in 파일을 읽어온다
            getChar(); //문자를 받아온다.
            do { 
                lex(); //문자열을 토큰에 따라 나눈다.
            }while( nextToken != EOF);
            fileReader.close();
        }
        

        catch (FileNotFoundException e) {
            // TODO: handle exception
        }
        catch (IOException e){
            System.out.println(e); //예외 처리
        }

    }
    

    public static void addChar() { //lexeme에 문자를 추가하는 함수
        if (lexLen <= 98) {
            lexeme += nextChar; //받아온 문자를 lexeme string에 넣는다.
            lexLen++;
           
        }
        else
            System.out.printf("Error - lexeme is too long \n");
    }

    public static void getChar() { //문자를 받아들여오는 함수
        try {
        ch = fileReader.read(); //파일을 읽어온다.
        }
        catch (FileNotFoundException e) {
            // TODO: handle exception
        }
        catch (IOException e){
            System.out.println(e);
        }

        if(ch != EOF) {
            nextChar = (char)ch; //파일내 문자를 하나씩 읽어온다.
            
            if(Character.isLetter(nextChar))
                charClass = LETTER; //만약 문자라면 LETTER를 charclass로 지정
            else if(Character.isDigit(nextChar))
                charClass = DIGIT; //만약 숫자라면 DIGIT를 charclass로 지정
            else 
                charClass = UNKNOWN; //미확인 문자에 대해서는 UNKNOWN으로 지정
        }
        else 
            charClass = EOF; //파일의 끝인 경우 EOF로 지정
        
    }

    public static void getNonBlank() {
        while(Character.isWhitespace(nextChar)) //공백문자 처리
        	getChar();
    }

    public static int lex() { //각 식들에 대해 토큰별로 판별
    lexLen = 0;
    getNonBlank();
    switch (charClass)
    {
        case LETTER :
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) { //charclass가 LETTER나 DIGIT이라면 
                addChar(); //문자를 lexeme string에 추가하고
                getChar(); //새 문자를 받아온다.
                
            }
            
            
            if(lexeme.equals("for")) {
                nextToken = FOR_CODE; //for문이면 FOR_CODE 지정
                break;
            }
            
            else if(lexeme.equals("if")) {
                nextToken = IF_CODE; // if문이면 IF_CODE 지정
                break;
            }
           
            else if(lexeme.equals("else")) {
                nextToken = ELSE_CODE; //else문이면 ELSE_CODE 지정
                break;
            }
            
            else if(lexeme.equals("while")) {
                nextToken = WHILE_CODE; //while문이면 WHILE_CODE 지정
                break;
            }
            
            else if(lexeme.equals("do")) {
                nextToken = DO_CODE; //do문이면 DO_CODE 지정
                break;
            }
            
            else if(lexeme.equals("int")) {
                nextToken = INT_CODE; //int면 INT_CODE 지정
                break;
            }
            
            else if(lexeme.equals("float")) {
                nextToken = FLOAT_CODE; //float이면 FLOAT_CODE 지정
                break;
            }
            
            else if(lexeme.equals("switch")) {
                nextToken = SWITCH_CODE; //switch면 SWITCH_CODE 지정
                break;
            }
            else {
            nextToken = IDENT; //그외는 identifier로 간주
            break;
            }
            

        case DIGIT :
            addChar();
            getChar();
            while (charClass == DIGIT) { //charclass가 DIGIT이면
                addChar(); //해당 숫자를 lexeme string에 넣고
                getChar(); //새로운 숫자를 받아온다
            }
            nextToken = INT_LIT; //INT_LIT으로 토큰 지정
            break;

        case UNKNOWN :
            lookup(nextChar); //미확인 문자에 대한 처리
            getChar();
            break;

        case EOF :
        nextToken = EOF; //파일의 끝에 도달한 경우 EOF 지정
        lexeme = "EOF";
        break;
    }
    /*End of Switch syntax*/
    System.out.println("Next token is : " + nextToken + ", "+ "Next lexeme is " + lexeme ); //토큰 반환 결과 출력
    lexeme = "";
    return nextToken;
    }

    public static int lookup(char ch) { //연산자에 대한 처리
        switch (ch)
        {
            case '(':
                addChar();
                nextToken = LEFT_PAREN; // "("이면 LEFT_PAREN으로 토큰 지정
                break;
    
            case ')':
                addChar();
                nextToken = RIGHT_PAREN; // ")"이면 RIGHT_PAREN으로 토큰 지정
                break;
    
            case '+':
                addChar();
                nextToken = ADD_OP; // "+"이면 ADD_OP으로 토큰 지정
                break;
    
            case '-' :
                addChar();
                nextToken = SUB_OP; // "-"이면 SUB_OP으로 토큰 지정
                break;
            
            case '*' :
                addChar();
                nextToken = MULT_OP; // "*"이면 MULT_OP으로 토큰 지정
                break;
    
            case '/' :
                addChar();
                nextToken = DIV_OP; // "/"이면 DIV_OP으로 토큰 지정
                break;
        
            default:
                addChar();
                nextToken = EOF; // 파일의 끝은 EOF로 지정
                break;
        }
        return nextToken;
    
    }
}
