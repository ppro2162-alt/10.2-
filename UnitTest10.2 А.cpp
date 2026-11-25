#include "pch.h"
#include "CppUnitTest.h"
#include <sstream>

// Pidklyuchayemo vash kod (struct Student, readInt, buildIndex tosho)
#include "C:\Users\USSER\Desktop\лаби\ап 10.2 А\ап 10.2 А\ап 10.2 А.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest102A
{
    // Dopomizhna funktsiya: stvoryuye studenta z potribnymy polyamy
    static Student mk(int num, const std::string& surname,
        const std::string& spec, int inf)
    {
        return Student{ num, surname, 1, spec, 50, 50, inf };
    }

    // Pidmina cin/cout na chas testu, shchob ne chekaty vvodu z klaviatury
    class IoRedirect {
    public:
        explicit IoRedirect(const std::string& input) : in_(input) {
            oldIn_ = std::cin.rdbuf(in_.rdbuf());
            oldOut_ = std::cout.rdbuf(out_.rdbuf());
        }
        ~IoRedirect() {
            std::cin.rdbuf(oldIn_);
            std::cout.rdbuf(oldOut_);
        }
    private:
        std::istringstream in_;
        std::ostringstream out_;
        std::streambuf* oldIn_;
        std::streambuf* oldOut_;
    };

    TEST_CLASS(SortingTests)
    {
    public:

        TEST_METHOD(IsLess_BySpecialty)
        {
            Student a[2] = { mk(1, "Zzz", "Ekonomika", 100),
                             mk(2, "Aaa", "Informatyka", 0) };
            Assert::IsTrue(isLessByIndex(a, 0, 1));
            Assert::IsFalse(isLessByIndex(a, 1, 0));
        }

        TEST_METHOD(IsLess_ByInformatics)
        {
            Student a[2] = { mk(1, "Zzz", "Informatyka", 70),
                             mk(2, "Aaa", "Informatyka", 80) };
            Assert::IsTrue(isLessByIndex(a, 0, 1));
            Assert::IsFalse(isLessByIndex(a, 1, 0));
        }

        TEST_METHOD(IsLess_BySurname)
        {
            Student a[2] = { mk(1, "Bondar", "Informatyka", 85),
                             mk(2, "Koval",  "Informatyka", 85) };
            Assert::IsTrue(isLessByIndex(a, 0, 1));
            Assert::IsFalse(isLessByIndex(a, 1, 0));
        }

        TEST_METHOD(IsLess_EqualRecords)
        {
            Student a[2] = { mk(1, "Koval", "Informatyka", 85),
                             mk(2, "Koval", "Informatyka", 85) };
            Assert::IsFalse(isLessByIndex(a, 0, 1));
            Assert::IsFalse(isLessByIndex(a, 1, 0));
        }

        TEST_METHOD(BuildIndex_SortsCorrectly)
        {
            Student a[5] = { mk(1, "Koval",      "Informatyka", 85),
                             mk(2, "Bondar",     "Informatyka", 85),
                             mk(3, "Melnyk",     "Ekonomika",   75),
                             mk(4, "Andriienko", "Informatyka", 70),
                             mk(5, "Lytvyn",     "Matematyka",  90) };
            int* idx = buildIndex(a, 5);

            Assert::AreEqual(2, idx[0]);   // Melnyk
            Assert::AreEqual(3, idx[1]);   // Andriienko
            Assert::AreEqual(1, idx[2]);   // Bondar
            Assert::AreEqual(0, idx[3]);   // Koval
            Assert::AreEqual(4, idx[4]);   // Lytvyn

            delete[] idx;
        }

        TEST_METHOD(BuildIndex_AlreadySorted)
        {
            Student a[3] = { mk(1, "A", "Ekonomika",   70),
                             mk(2, "B", "Informatyka", 80),
                             mk(3, "C", "Matematyka",  90) };
            int* idx = buildIndex(a, 3);
            Assert::AreEqual(0, idx[0]);
            Assert::AreEqual(1, idx[1]);
            Assert::AreEqual(2, idx[2]);
            delete[] idx;
        }

        TEST_METHOD(BuildIndex_ReverseOrder)
        {
            Student a[3] = { mk(1, "C", "Matematyka",  90),
                             mk(2, "B", "Informatyka", 80),
                             mk(3, "A", "Ekonomika",   70) };
            int* idx = buildIndex(a, 3);
            Assert::AreEqual(2, idx[0]);
            Assert::AreEqual(1, idx[1]);
            Assert::AreEqual(0, idx[2]);
            delete[] idx;
        }

        TEST_METHOD(BuildIndex_SingleElement)
        {
            Student a[1] = { mk(1, "Koval", "Informatyka", 85) };
            int* idx = buildIndex(a, 1);
            Assert::AreEqual(0, idx[0]);
            delete[] idx;
        }

        TEST_METHOD(BuildIndex_DoesNotChangeOriginalArray)
        {
            Student a[3] = { mk(1, "C", "Matematyka",  90),
                             mk(2, "B", "Informatyka", 80),
                             mk(3, "A", "Ekonomika",   70) };
            int* idx = buildIndex(a, 3);

            Assert::IsTrue(a[0].surname == "C" && a[0].number == 1);
            Assert::IsTrue(a[1].surname == "B" && a[1].number == 2);
            Assert::IsTrue(a[2].surname == "A" && a[2].number == 3);

            delete[] idx;
        }
    };

    TEST_CLASS(InputTests)
    {
    public:

        TEST_METHOD(ReadInt_ValidValue)
        {
            IoRedirect io("5\n");
            Assert::AreEqual(5, readInt("x: ", 1, 10));
        }

        TEST_METHOD(ReadInt_Boundaries)
        {
            IoRedirect io("1\n10\n");
            Assert::AreEqual(1, readInt("x: ", 1, 10));
            Assert::AreEqual(10, readInt("x: ", 1, 10));
        }

        TEST_METHOD(ReadInt_OutOfRangeThenValid)
        {
            IoRedirect io("0\n11\n-3\n7\n");
            Assert::AreEqual(7, readInt("x: ", 1, 10));
        }

        TEST_METHOD(ReadInt_NotANumberThenValid)
        {
            IoRedirect io("abc\n\n3.5\n12abc\n4\n");
            Assert::AreEqual(4, readInt("x: ", 1, 10));
        }

        TEST_METHOD(ReadNonEmptyString_TrimsSpaces)
        {
            IoRedirect io("  Koval \t\n");
            Assert::IsTrue(readNonEmptyString("s: ") == "Koval");
        }

        TEST_METHOD(ReadNonEmptyString_EmptyThenValid)
        {
            IoRedirect io("\n   \nBondar\n");
            Assert::IsTrue(readNonEmptyString("s: ") == "Bondar");
        }

        TEST_METHOD(ReadStudent_ValidInput)
        {
            IoRedirect io("Koval\n3\nInformatyka\n80\n90\n95\n");
            Student s = readStudent(4);
            Assert::AreEqual(4, s.number);
            Assert::IsTrue(s.surname == "Koval");
            Assert::AreEqual(3, s.course);
            Assert::IsTrue(s.specialty == "Informatyka");
            Assert::AreEqual(80, s.physics);
            Assert::AreEqual(90, s.math);
            Assert::AreEqual(95, s.informatics);
        }

        TEST_METHOD(ReadStudent_InvalidThenValid)
        {
            // porozhnye prizvyshche, kurs 9 i "x", otsinka 101 i -1 -> potim pravylni
            IoRedirect io("\nKoval\n9\nx\n2\nInformatyka\n101\n70\n80\n-1\n90\n");
            Student s = readStudent(1);
            Assert::AreEqual(2, s.course);
            Assert::AreEqual(70, s.physics);
            Assert::AreEqual(80, s.math);
            Assert::AreEqual(90, s.informatics);
        }

        TEST_METHOD(FillStudents_SetsNumbers)
        {
            IoRedirect io("A\n1\nEkonomika\n10\n20\n30\n"
                "B\n2\nMatematyka\n40\n50\n60\n");
            Student* a = createStudents(2);
            fillStudents(a, 2);
            Assert::AreEqual(1, a[0].number);
            Assert::AreEqual(30, a[0].informatics);
            Assert::AreEqual(2, a[1].number);
            Assert::AreEqual(60, a[1].informatics);
            delete[] a;
        }
    };
}