/***
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-07-13 19:18:32
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-07-15 02:18:35
 * @FilePath: /Vector/src/MyVector.hpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ls02, All Rights Reserved.
 */
/***
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-07-13 18:54:57
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-07-13 18:54:58
 * @FilePath: /Vector/src/MyVector.hpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ls02, All Rights Reserved.
 */
/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 *
 *
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *            佛祖保佑     永不宕机     永无BUG
 *
 *        佛曰:
 *                写字楼里写字间，写字间里程序员；
 *                程序人员写程序，又拿程序换酒钱。
 *                酒醒只在网上坐，酒醉还来网下眠；
 *                酒醉酒醒日复日，网上网下年复年。
 *                但愿老死电脑间，不愿鞠躬老板前；
 *                奔驰宝马贵者趣，公交自行程序员。
 *                别人笑我忒疯癫，我笑自己命太贱；
 *                不见满街漂亮妹，哪个归得程序员？
 */

#ifndef __MY_VECTOR_HPP__
#define __MY_VECTOR_HPP__

namespace ls
{
    template <class T>
    class vector
    {
    public:
        typedef T *iterator;

    public:
        // 默认构造
        vector()
            : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr)
        {
        }

        // 插入
        void push_back(const T &value)
        {
            if (size() == capacity())
            {
                // 如果空间满了就需要扩容
            }
        }

        size_t size() const
        {
            return _finish - _start;
        }

        size_t capacity() const
        {
            return _end_of_storage - _start;
        }

    private:
        iterator _start;
        iterator _finish;
        iterator _end_of_storage;
    };

}

#endif