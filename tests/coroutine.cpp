#include <gtest/gtest.h>

import bembel;

using namespace bembel::base::coro;

class CoroTest : public testing::Test {
  protected:
    CoroTest() {
        //* 
        bembel::base::Logger::debug.setLogSink(nullptr);
        //*/
        ALIVE_COROUTINES.clear();
    }
    ~CoroTest() {}

  public:
    static std::size_t onNewCoroutine() {
        std::scoped_lock lock{CoroTest::ALIVE_COROUTINES_MUTEX};
        std::size_t      coro_index = ALIVE_COROUTINES.size();
        ALIVE_COROUTINES.push_back(true);
        return coro_index;
    }
    static void onDeleteCoroutine(std::size_t coro_index) {
        std::scoped_lock lock{CoroTest::ALIVE_COROUTINES_MUTEX};
        ALIVE_COROUTINES[coro_index] = false;
    }

  protected:
    static std::vector<bool> ALIVE_COROUTINES;
    static std::mutex        ALIVE_COROUTINES_MUTEX;
};
std::vector<bool> CoroTest::ALIVE_COROUTINES;
std::mutex        CoroTest::ALIVE_COROUTINES_MUTEX;

template <typename T>
class PromiseMock : public Promise<T> {
  public:
    using Handle = CoroutineHandle<PromiseMock<T>>;
    
    PromiseMock() : m_coro_index{CoroTest::onNewCoroutine()} {
        bembel::base::logDebug("Creating Coroutine {}", m_coro_index);
    }
    ~PromiseMock() {
        bembel::base::logDebug("Deleting Coroutine {}", m_coro_index);
        CoroTest::onDeleteCoroutine(m_coro_index);
    }
  public:
    std::size_t const m_coro_index;
};

template <typename T>
using TaskMock = Task<T, PromiseMock<T>>;

TaskMock<void> emptyCoro() {
    bembel::base::logDebug("Run empty Coroutine");
    co_return;
}

TaskMock<int> geratorCoro() {
    for(int i = 1; true; ++i) {
        bembel::base::logDebug("Run Generator Coroutine (iteration {})", i);
        co_yield i;
    }
}

TEST_F(CoroTest, createCoro) {
    auto task = emptyCoro();

    EXPECT_EQ(1, task.getPromise().reference_count);
    EXPECT_FALSE(task.isDone());
    EXPECT_EQ(1, ALIVE_COROUTINES.size());
    EXPECT_TRUE(ALIVE_COROUTINES[0]);
}

TEST_F(CoroTest, deleteCoro) {
    { auto task = emptyCoro(); }

    EXPECT_EQ(1, ALIVE_COROUTINES.size());
    EXPECT_FALSE(ALIVE_COROUTINES[0]);
}

TEST_F(CoroTest, resumeCoro) {
    auto task = emptyCoro();
    task.resume();

    EXPECT_EQ(1, task.getPromise().reference_count);
    EXPECT_TRUE(task.isDone());
    EXPECT_EQ(1, ALIVE_COROUTINES.size());
    EXPECT_TRUE(ALIVE_COROUTINES[0]);
}

TEST_F(CoroTest, yieldResult) {
    auto task = geratorCoro();
    task.resume();

    EXPECT_FALSE(task.isDone());
    EXPECT_TRUE(task.hasResult());
    EXPECT_EQ(1, task.getResult());
}

TEST_F(CoroTest, generator) {
    auto task = geratorCoro();

    int a = task();
    int b = task();
    int c = task();

    EXPECT_EQ(1, a);
    EXPECT_EQ(2, b);
    EXPECT_EQ(3, c);
    EXPECT_FALSE(task.isDone());
}

TEST_F(CoroTest, recursiveCoro) {
    auto outerCoro = []() -> TaskMock<int> {
        auto innerCoro = []() -> TaskMock<int> {
            bembel::base::logDebug("Run Inner-Coroutine");
            co_return 42;
        };
        bembel::base::logDebug("Run Outer-Coroutine");
        int result = co_await innerCoro();
        bembel::base::logDebug("Continue Outer-Coroutine");
        co_return result;
    };
    auto task = outerCoro();

    int result = task();

    EXPECT_EQ(42, result);
    EXPECT_TRUE(task.isDone());
    EXPECT_EQ(2, ALIVE_COROUTINES.size());
    EXPECT_TRUE(ALIVE_COROUTINES[0]);
    EXPECT_FALSE(ALIVE_COROUTINES[1]);
}

TEST_F(CoroTest, task_resume_should_rethrow_exeption) {
    auto coro1= []() -> TaskMock<void> {
        throw std::exception("Test");
        co_return;
    };
    auto coro2 = []() -> TaskMock<int> {
        throw std::exception("Test");
        co_return 42;
    };
    auto task1 = coro1();
    auto task2 = coro2();

    ASSERT_THROW(task1.resume(), std::exception);
    ASSERT_THROW(task2.resume(), std::exception);
}

TEST_F(CoroTest, TaskAwaiter_should_rethrow_exeption) {
    auto outerCoro = []() -> TaskMock<void> {

        auto innerCoro = []() -> TaskMock<void> {
            throw std::exception("Test");
            co_return;
        };
        
        co_await innerCoro();

        co_return;
    };
    auto task = outerCoro();

    ASSERT_THROW(task.resume(), std::exception);
}