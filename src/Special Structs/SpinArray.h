
    template <typename T>
    class SpinArray{

        public:
            //Constructor
            SpinArray(const int arraySize) {
                length = arraySize;

                top = new T*[arraySize];
                head = 0;
                tail = 0;

                overwrite = false;
                overflow = false;
                lossCounter = 0;
            };

            //Destructor
            ~SpinArray() {
                for (int i=0; i<length; i++)
                    if (top[i] != nullptr)
                        delete top[i];

                delete[] top;
            };

            //flag gets
            bool isOverwriting() { return overwrite; };
            bool isOverflowing() { return overflow; };
            long getLossCount() { return lossCounter; };

            //flag sets
            void setOverwrite(bool over) { overwrite = over; };
            void resetCount() { lossCounter = 0; };

            bool put(T* item) {
                if (overflow && !overwrite) {
                    lossCounter++;
                    delete item;
                    return false;
                }

                top[head++] = item;

                if (head == length)
                    head = 0;

                if (head == tail || top[head] != nullptr)
                    overflow = true;
            };

            T* get() {
                T* item = top[tail];
                top[tail++] = nullptr;

                if (item  != nullptr) {
                    if (tail >= length)
                        tail = 0;
                }

                return item;
            };

            void cear() {
                for (int i=0; i<length; i++)
                    if (top[i] != nullptr)
                        delete top[i];

                head = 0;
                top = 0;
                lossCounter = 0;
            };

            void print() {
                for (int i=0; i<length; i++) {
                    if (i == head)
                        std::cout << " [H] ";
                    if (i == tail)
                        std::cout << " [T] ";
                    if (top[i] != nullptr)
                        std::cout << " [X] ";
                    else
                        std::cout << " [ ] ";
                }
                std::cout << std::endl;

            };

        private:
            //flags
            bool overwrite;
            bool overflow;
            long lossCounter;

            T** top;
            long head;
            long tail;
            long length;





    };

