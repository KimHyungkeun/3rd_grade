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
   
    static final int EOF = -1;
    static final int LETTER = 0;
    static final int DIGIT = 1;
    static final int UNKNOWN = 99;
    
    static final int INT_LIT = 10;
    static final int IDENT = 11;
    static final int ASSIGN_OP = 20;
    static final int ADD_OP = 21;
    static final int SUB_OP = 22;
    static final int MULT_OP = 23;
    static final int DIV_OP = 24;
    static final int LEFT_PAREN = 25;
    static final int RIGHT_PAREN = 26;
    
    static final int FOR_CODE = 30;
    static final int IF_CODE = 31;
    static final int ELSE_CODE = 32;
    static final int WHILE_CODE = 33;
    static final int DO_CODE = 34;
    static final int INT_CODE = 35;
    static final int FLOAT_CODE = 36;
    static final int SWITCH_CODE = 37;
    
        

	public static void main(String[] args){

       

        try{
            
            fileReader = new FileReader(new File("front.in"));
            getChar();
            do {
                lex();
            }while( nextToken != EOF);
            fileReader.close();
        }
        

        catch (FileNotFoundException e) {
            // TODO: handle exception
        }
        catch (IOException e){
            System.out.println(e);
        }

    }
    

    public static void addChar() {
        if (lexLen <= 98) {
            lexeme += nextChar;
            lexLen++;
           
        }
        else
            System.out.printf("Error - lexeme is too long \n");
    }

    public static void getChar() {
        try {
        ch = fileReader.read();
        }
        catch (FileNotFoundException e) {
            // TODO: handle exception
        }
        catch (IOException e){
            System.out.println(e);
        }

        if(ch != EOF) {
            nextChar = (char)ch;
            
            if(Character.isLetter(nextChar))
                charClass = LETTER;
            else if(Character.isDigit(nextChar))
                charClass = DIGIT;
            else
                charClass = UNKNOWN;
        }
        else 
            charClass = EOF;
        
    }

    public static void getNonBlank() {
        while(Character.isWhitespace(nextChar))
        
        getChar();
    }

    public static int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass)
    {
        case LETTER :
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
                
            }
            
            
            if(lexeme.equals("for")) {
                nextToken = FOR_CODE;
                break;
            }
            
            else if(lexeme.equals("if")) {
                nextToken = IF_CODE;
                break;
            }
           
            else if(lexeme.equals("else")) {
                nextToken = ELSE_CODE;
                break;
            }
            
            else if(lexeme.equals("while")) {
                nextToken = WHILE_CODE;
                break;
            }
            
            else if(lexeme.equals("do")) {
                nextToken = DO_CODE;
                break;
            }
            
            else if(lexeme.equals("int")) {
                nextToken = INT_CODE;
                break;
            }
            
            else if(lexeme.equals("float")) {
                nextToken = FLOAT_CODE;
                break;
            }
            
            else if(lexeme.equals("switch")) {
                nextToken = SWITCH_CODE;
                break;
            }
            else {
            nextToken = IDENT;
            break;
            }
            

        case DIGIT :
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        case UNKNOWN :
            lookup(nextChar);
            getChar();
            break;

        case EOF :
        nextToken = EOF;
        lexeme = "EOF";
        break;
    }
    /*End of Switch syntax*/
    System.out.println("Next token is : " + nextToken + ", "+ "Next lexeme is " + lexeme );
    lexeme = "";
    return nextToken;
    }

    public static int lookup(char ch) {
        switch (ch)
        {
            case '(':
                addChar();
                nextToken = LEFT_PAREN;
                break;
    
            case ')':
                addChar();
                nextToken = RIGHT_PAREN;
                break;
    
            case '+':
                addChar();
                nextToken = ADD_OP;
                break;
    
            case '-' :
                addChar();
                nextToken = SUB_OP;
                break;
            
            case '*' :
                addChar();
                nextToken = MULT_OP;
                break;
    
            case '/' :
                addChar();
                nextToken = DIV_OP;
                break;
        
            default:
                addChar();
                nextToken = EOF;
                break;
        }
        return nextToken;
    
    }
}
