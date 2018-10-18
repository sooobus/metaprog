#include <iostream>
#include <vector>
#include <memory>

/* Proxy, mediator, observer
 * О задании подробнее:
Есть два класса, А и В.
У каждого из них есть свой публичный интерфейс.
Нужно написать новый шаблонный класс С, который в зависимости от шаблонного параметра будет выполнять разные роли.
1. Прокси
Обращение объектов класса А к объектам класса В должно выполняться не напрямую,
 а через аналогичный интерфейс класса-прокси.
2. Медиатор
Класс С должен иметь возможность хранить целый набор объектов класса В(или указателей на них)
 и в случае обращения от объекта класса А автоматически выбирать объект класса В и передавать ему запрос.
3. Наблюдатель
Должна появиться возможность для объектов класса А подписываться на объекты класса В.
 Тогда при обновлении объекта класса В объект С должен уведомлять всех подписчиков этого объекта-объекты класса А

 Объект С просто должен иметь возможность узнать у объекта В, было ли какое-то изменение.
Либо можно сделать, что объект В сохраняет ссылку на С, и уведомляет его об изменении какого-то свойства.
 Далее С уведомляет тех А, кто подписан на изменение этого конкретного свойства

 */

class C; // Forward declaration
class B;
class A;

class B {
public:
    explicit B(size_t id_); // B must not know anything about other classes, but this id is included to demonstrate its work.


    void Foo(); // Example function that is needed to be run from A (for proxy & mediator).
    size_t GetState(); // Example state is just a bool that shows that Foo was called at least once.
    
private:
    bool was_called;
    size_t id; 
};

class C {
public:
    explicit C(size_t b_number); // Number of workers (objects of class B) is known before.

    void BFoo(); // Used as interface for calling A.Foo.
    void AddListener(std::shared_ptr<A> ptr); // Adds new A that receives notifications.
    void AskBState(size_t index); // Asks specific B about its state.

private:
    std::vector<std::unique_ptr<B>> b_ptrs;
    std::vector<std::shared_ptr<A>> listeners;
    size_t call_index;
};

class A {
public:
    A(C* multitool_); // Uses C as proxy and / or mediator.

    void CallFoo(); // Demonstrates calling B.Foo().
    void GetNotification(bool x); // Receives information about states that it is subscribed to.

private:
    C* multitool;
};

void A::GetNotification(bool x){
    if(x){
        std::cout << "Received state notification from B: it was already called" << std::endl;
    }
    else{
        std::cout << "Received state notification from B: it has not been called yet" << std::endl;
    }
}

A::A(C* multitool_) : multitool(multitool_) {}

B::B(size_t id_) : id(id_), was_called(false) {}

C::C(size_t b_number) : call_index(0) {
    b_ptrs.reserve(b_number);
    for (size_t i = 0; i < b_number; ++i)
        b_ptrs.emplace_back(new B(i));
}

void A::CallFoo() {
    multitool->BFoo();
}

size_t B::GetState(){
    return was_called;
}

void B::Foo() {
    std::cout << "Yes ma'm, my number is " << id << std::endl;
    was_called = true;
}

void C::AskBState(size_t index){
    for(auto listener : listeners){
        listener->GetNotification(b_ptrs[index]->GetState());
    }
}

void C::BFoo() {
    b_ptrs[call_index]->Foo();
    call_index++;
    call_index %= b_ptrs.size();
}

void C::AddListener(std::shared_ptr<A> ptr){
    listeners.push_back(ptr);
}

int main() {
    C c(5);
    A a1(&c);
    A a2(&c);
    A a3(&c);

    c.AddListener(std::make_shared<A>(a1));
    c.AddListener(std::make_shared<A>(a1));

    for(size_t i = 0; i < 5; ++i)
        c.AskBState(i);

    a1.CallFoo();
    
    for(size_t i = 0; i < 5; ++i)
        c.AskBState(i);

    a2.CallFoo();
    
    for(size_t i = 0; i < 5; ++i)
        c.AskBState(i);

    a3.CallFoo();
    for(size_t i = 0; i < 5; ++i)
        c.AskBState(i);

    return 0;
}
