class SharedDataPrinter {
  synchronized public void display(String str) {
    for (int i = 0; i < str.length(); i++) {
      System.out.print(str.charAt(i));
      try {
        Thread.sleep(100);
      } catch (Exception e) {
      }
    }
  }
}

class Thread1 extends Thread {
  SharedDataPrinter p;

  public Thread1(SharedDataPrinter p) {
    this.p = p;
  }

  public void run() {
    p.display("Java");
  }
}

class Thread2 extends Thread {
  SharedDataPrinter p;

  public Thread2(SharedDataPrinter p) {
    this.p = p;
  }

  public void run() {
    p.display(" monitor");
  }
}

class Main {
  public static void main(String[] args) {
    SharedDataPrinter printer = new SharedDataPrinter();

    Thread1 t1 = new Thread1(printer);
    Thread2 t2 = new Thread2(printer);

    t1.start();
    t2.start();
  }
}