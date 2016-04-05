struct A {
  A() { foo(); }
  virtual void foo() { print "A"; }
};

struct B : public A {
  B() { foo(); }
  virtual void foo() { print "B"; }
};

struct C : public B {
  C() { foo(); }
  virtual void foo() { print "C"; }
};

C c; // In Java: CCC, in C++: ABC
