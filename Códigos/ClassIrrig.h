class Valvula{
  private:
    boolean sts;
    int id;
  
  public:
    Valvula(int idv){
      sts = true;
      id = idv;
      digitalWrite(id, sts);
    }
    void ligar(){
      sts = false;
      digitalWrite(id, sts);
    }
    void desligar(){
      sts = true;
      digitalWrite(id, sts);
    }
    void setStatus(){
      sts = !sts;
      digitalWrite(id, sts);
    }
    boolean getStatus(){
      return sts;
      }
    int getId(){
      return id;
      }    
};