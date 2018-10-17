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

class B {
public:
    explicit B(size_t i);

    void Foo();

    std::unique_ptr<C> observer;
    size_t id;
};

class C {
public:
    explicit C(size_t b_number);

    void BFoo();

    std::vector<std::unique_ptr<B>> b_ptrs;
    size_t call_index;
};

class A {
public:
    A();

    void CallFoo();

    std::unique_ptr<C> multitool;
};


B::B(size_t i) : id(i) {};

void B::Foo() {
    std::cout << "Yes ma'm, my number is " << id << std::endl;
}

C::C(size_t b_number) : call_index(0) {
    b_ptrs.reserve(b_number);
    for (size_t i = 0; i < b_number; ++i)
        b_ptrs.emplace_back(new B(i));
}

void C::BFoo() {
    b_ptrs[call_index]->Foo();
    call_index++;
    call_index %= b_ptrs.size();
}


A::A() : multitool(new C(5)) {
}

void A::CallFoo() {
    multitool->BFoo();
}

int main() {
    A a;
    a.CallFoo();
    a.CallFoo();
    a.CallFoo();
    a.CallFoo();
    a.CallFoo();
    a.CallFoo();
    return 0;
}