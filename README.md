В папке “<STRONG>program</STRONG>” находится исполняемый файл, а в папке “<STRONG>program/TEST</STRONG>” исполняемый файл для тестирования. В главном “<STRONG>CMakeLists.txt</STRONG>” есть строчка для переключения между “<STRONG>program</STRONG>” и “<STRONG>program/TEST</STRONG>”.

initialization
---
`bool initialization(string& error)`

При старте программы запускается функция `bool initialization(string& error)` и идёт проверка:
1) Наличие “<STRONG>config.json</STRONG>”. Ошибка “<STRONG>config file is missing</STRONG>”.
2) Проверка, что “<STRONG>config.json</STRONG>” содержит данные. Ошибка “<STRONG>config file is empty</STRONG>”.
<EM><STRONG>Примечание!</STRONG> Отсутствие в файле “<STRONG>config.json</STRONG>” пары “<STRONG>config</STRONG>” не допускается.
</EM>

function/converter JSON
---
`vector<string> GetTextDocuments()`

`unsigned short GetResponsesLimit()`

`vector<string> GetRequests()`

`void putAnswers(vector<SearchServer> relevance)`

Управляющим классом является “<STRONG>ConverterJSON</STRONG>” и состоит из следующих функций:
1) `vector<string> GetTextDocuments()` проверяет в файле “<STRONG>config.json</STRONG>” версию и при не совпадении выдаёт предупреждение “<STRONG>has incorrect file version</STRONG>”. Далее ищет файлы указанные в “<STRONG>config.json</STRONG>” и при отсутствии выдаёт предупреждение “<адрес> <STRONG>not found</STRONG>”. В конце выдаёт файлы в виде строк vector.
2) `unsigned short GetResponsesLimit()` в файле “<STRONG>config.json</STRONG>” ищет и выдаёт значение “<STRONG>max_responses</STRONG>”.
3) `vector<string> GetRequests()` в файле “<STRONG>requests.json</STRONG>” читает список “<STRONG>requests</STRONG>” и выдаёт в виде строк vector.
4) `void putAnswers(vector<SearchServer> relevance)` использует вспомогательные классы:
- “<STRONG>auxiliary resource/print JSON</STRONG>” ответственный за разметку JSON текста.
- “<STRONG>auxiliary resource/search server</STRONG>” ответственный за обработку данных.

После успешной обработки сохраняет результат, как новый файл “<STRONG>answers.json</STRONG>”.

auxiliary resource/print JSON
---
`PrintJSON(std::string& operGET) : GET(operGET)`

`operator(const char* kay)`

`operator(const char* kay, const char* comment)`

`operator(const char* kay, float comment, unsigned char accuracy = 8)`

`--operator`

`operator--`

Модуль <STRONG>“PrintJSON</STRONG>” в начале принимает ссылку на <STRONG>string</STRONG>, а дальше формирует текст по следующим запросам:
(kay = first, comment = second)
1) `operator(const char* kay)` – создаёт ключ (массив).
2) `operator(const char* kay, const char* comment)` – создаёт пару.
3) `operator(const char* kay, float comment, unsigned char accuracy = 8)` – создаёт пару с плавающей запятой. Можно настроить точность по количеству цифр (по умолчанию 8).
4) `--object` – закрывает массив пар.
5) `object--` – закрывает все пары (завершение).
6) Для проверки или возвращения окончательного результата достаточно воспользоваться “<STRONG>operator()</STRONG>”, вернёт “<STRONG>string</STRONG>”.
При этом стоит отметить, что функционал рассчитан только для выполнения текущей задачи и тут отсутствует такой функционал, как “<STRONG>operator(comment)</STRONG>”.

<EM><STRONG>Примечание!</STRONG> Классы в “<STRONG>stream operator</STRONG>” обычно не чистят себя и должны быть ликвидированы после получения результата.
</EM>

global resource/manager
---
`inline void manager(auto& object, auto& first, auto& size)`

`inline void manager(auto& object, auto& first, auto& size, auto& secon)`

В проекте реализована система автоматической многопоточности “<STRONG>manager(...)</STRONG>” по средствам дробления массива на физические потоки. Сама функция примитивна и не контролирует равномерное распределение загруженности на долгий промежуток времени. Функция принимает такие виды данных: `(&,&,&,&) и (&,&,&)`.
Где 1 – объект взаимодействия (рекомендуется <STRONG>vector</STRONG>), 2 – объект передачи данных (рекомендуется <STRONG>vector</STRONG>), 3 – длина объекта (1,3), 4 – шаблонные данные (рекомендуется <STRONG>vector</STRONG>).
В однопоточном представлении функция выглядит как:

	for(i = 0; i < max; ++i){
		object[i] = first[i];// для 3 и 4 значений.
		object[i](second);// для 4 значений.
	}
При этом, если во втором значении будет 1, то потоки не будут созданы, а если значение будет меньше количества физических потоков, то будет создано то количество потоков, которое возможно согласно максимальному количеству второго значения.

auxiliary resource/search server
---
`void operator= const string& list`

`void operator(map<string, vector<Index>>& index)`

`map<string, vector<unsigned short>>& operator()`

Модуль “<STRONG>SearchServer</STRONG>” написан строго для “<STRONG>thread/manager</STRONG>” из 4 значений. Данный модуль производить поиск данных по индексированному массиву “<STRONG>stream operator/inverted index</STRONG>”.
В первой фазе оператор “<STRONG>=</STRONG>” принимает “<STRONG>string</STRONG>”, где строка эта набор слов, которую надо разделить на слова.
Во второй фазе оператор “<STRONG>()</STRONG>” принимает `map<string, vector<Index>>`, где первое значение это уникальное слово, а второе это “<STRONG>vector</STRONG>” из двух “<STRONG>unsigned short</STRONG>” значений (ID и count). В конце за кэшированные данные первого этапа полностью удаляются!
Для получения результата (ссылки) используется “<STRONG>operator()</STRONG>”, но так как он нужен только для “<STRONG>function/converter JSON</STRONG>”, то достаточно поместить его в функцию `void putAnswers(vector<SearchServer> relevance)`, что бы завершить его работу.

auxiliary resource/inverted index
---
`InvertedIndex (vector<string>& files)`

`const map<string, vector<Index>>& operator()`

Модуль “<STRONG>InvertedIndex</STRONG>” состоит из одной фазы на этапе инициализации “<STRONG>()</STRONG>”, которая принимает `vector<string>` или результат функции `GetTextDocuments()` класса “<STRONG>function/converter JSON</STRONG>”. Так же внутри класса присутствует поддержка многопоточности.
Для получения результата (ссылки) используется “<STRONG>operator()</STRONG>”, который возвращает `map<string, vector<Index>>&`.