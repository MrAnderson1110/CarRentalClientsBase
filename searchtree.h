#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include <typeinfo>
#include <cstring>

#include "global.h"
#include "linklist.h"

using appGlobal::uint;

template<typename Val, uint KeyLen>
class Tree {

    struct Node {
        Node(const Val &data = Val(), const char *key = nullptr, Node *right = nullptr, Node *left = nullptr)
            : m_data(data)
            , m_key(new char[KeyLen])
            , m_height(1)
            , m_rightChild(right)
            , m_leftChild(left) { if(key) std::strncpy(m_key, key, KeyLen); }
        ~Node() { delete[] m_key; }

        int balanceFactor() const
        {
            uint leftHeight = m_leftChild ? m_leftChild->m_height : 0;
            uint rightHeight = m_rightChild ? m_rightChild->m_height : 0;

            return rightHeight - leftHeight;
        }
        void fixHeight()
        {
            uint leftHeight = m_leftChild ? m_leftChild->m_height : 0;
            uint rightHeight = m_rightChild ? m_rightChild->m_height : 0;

            m_height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
        }
        Node *rotateLeft()
        {
            Node *rotNode = m_rightChild;
            m_rightChild = rotNode->m_leftChild;
            rotNode->m_leftChild = this;
            fixHeight();
            rotNode->fixHeight();
            return rotNode;
        }
        Node *rotateRight()
        {
            Node *rotNode = m_leftChild;
            m_leftChild = rotNode->m_rightChild;
            rotNode->m_rightChild = this;
            fixHeight();
            rotNode->fixHeight();
            return rotNode;
        }
        Node *balance()
        {
            fixHeight();
            if(balanceFactor() == 2) {
                if(m_rightChild->balanceFactor() < 0)
                    m_rightChild = m_rightChild->rotateRight();
                return rotateLeft();
            }
            else if(balanceFactor() == -2) {
                if(m_leftChild->balanceFactor() > 0)
                    m_leftChild = m_leftChild->rotateLeft();
                return rotateRight();
            }
            return this;
        }
        Node *findMinNode()
        {
            return m_leftChild ? m_leftChild->findMinNode() : this;
        };
        Node *removeMinNode()
        {
            if(!m_leftChild)
                return m_rightChild;
            m_leftChild = m_leftChild->removeMinNode();
            return balance();
        }
        Node *insertNode(const char *key, const Val &val)
        {
            if(std::strncmp(key, m_key, KeyLen) < 0) {
                if(m_leftChild)
                    m_leftChild = m_leftChild->insertNode(key, val);
                else
                    m_leftChild = new Node(val, key);
            }
            else if(std::strncmp(key, m_key, KeyLen) > 0) {
                if(m_rightChild)
                    m_rightChild = m_rightChild->insertNode(key, val);
                else
                    m_rightChild = new Node(val, key);
            }
            return balance();
        }
        Node *eraceNode(const char *key)
        {
            if(std::strncmp(key, m_key, KeyLen) < 0)  {
                if(m_leftChild != nullptr)
                    m_leftChild = m_leftChild->eraceNode(key);
                //            else
                //                return node;
            }
            else if(std::strncmp(key, m_key, KeyLen) > 0) {
                if(m_rightChild != nullptr)
                    m_rightChild = m_rightChild->eraceNode(key);
                //            else
                //                return node;
            }
            else  {
                Node *leftNode = m_leftChild;
                Node *rightNode = m_rightChild;

                delete this;

                if(!rightNode) return leftNode;

                Node *minNode = rightNode->findMinNode();
                minNode->m_rightChild = rightNode->removeMinNode();
                minNode->m_leftChild = leftNode;
                return minNode->balance();
            }
            return balance();
        }
        void valCrawling(LinkList<Val> &storage)
        {
            if(m_leftChild)
                m_leftChild->valCrawling(storage);
            if(m_rightChild)
                m_rightChild->valCrawling(storage);
            storage.append(m_data);
        }

        Val m_data;
        char *m_key;
        uint m_height;
        Node *m_rightChild;
        Node *m_leftChild;
    };

public:
    Tree() : m_rootNode(nullptr), m_size(0) {}
    ~Tree() { clear(); };

    const Val &operator[](const char *key) const;
    Val &operator[](const char *key);

    Val value(const char *key) const;
    LinkList<Val> values() const;

    bool hasKey(const char *key) const;

    void insert(const char *key, const Val &val);
    void erase(const char *key);
    void clear();

    uint size() const { return m_size; }

private:
    Node *m_rootNode;
    uint m_size;
};

template<typename Val, uint KeyLen>
Val &Tree<Val, KeyLen>::operator[](const char *key)
{
    Node *searchNode = m_rootNode;
    while(searchNode && std::strncmp(key, searchNode->m_key, KeyLen) != 0) {
        if(std::strncmp(key, searchNode->m_key, KeyLen) > 0) {
            if(!searchNode->m_rightChild)
                searchNode = nullptr;
            else
                searchNode = searchNode->m_rightChild;
        }
        else {
            if(!searchNode->m_leftChild)
                searchNode = nullptr;
            else
                searchNode = searchNode->m_leftChild;
        }
    }
    if(searchNode)
        return searchNode->m_data;
    else {
        Q_ASSERT_X(searchNode, "Tree::get", "Attemp access to nonexistent node");
        return m_rootNode->m_data;
    }
}

template<typename Val, uint KeyLen>
const Val &Tree<Val, KeyLen>::operator[](const char *key) const
{
    return operator[](key);
}

template<typename Val, uint KeyLen>
Val Tree<Val, KeyLen>::value(const char *key) const
{
    if(!m_rootNode)
        return Val();

    Node *searchNode = m_rootNode;
    while(searchNode) {
        if(std::strncmp(key, searchNode->m_key, KeyLen) > 0)
            searchNode = searchNode->m_rightChild;
        else if(std::strncmp(key, searchNode->m_key, KeyLen) < 0)
            searchNode = searchNode->m_leftChild;
        else
            return searchNode->m_data;
    }
    Q_ASSERT_X(searchNode, "Tree::get", "Attemp access to nonexistent node");
    return Val();

}

template<typename Val, uint KeyLen>
LinkList<Val> Tree<Val, KeyLen>::values() const
{
    LinkList<Val> storage;
    if(!m_rootNode)
        return storage;

    m_rootNode->valCrawling(storage);
    return storage;
}

template<typename Val, uint KeyLen>
bool Tree<Val, KeyLen>::hasKey(const char *key) const
{
    if(!m_rootNode)
        return false;

    Node *searchNode = m_rootNode;
    while(searchNode) {
        if(std::strncmp(key, searchNode->m_key, KeyLen) > 0)
            searchNode = searchNode->m_rightChild;
        else if(std::strncmp(key, searchNode->m_key, KeyLen) < 0)
            searchNode = searchNode->m_leftChild;
        else
            return true;
    }
    return false;
}

template<typename Val, uint KeyLen>
void Tree<Val, KeyLen>::insert(const char *key, const Val &val)
{
    ++m_size;
    if(!m_rootNode) {
        m_rootNode = new Node(val, key);
        return;
    }
    m_rootNode = m_rootNode->insertNode(key, val);
}

template<typename Val, uint KeyLen>
void Tree<Val, KeyLen>::erase(const char *key)
{
    --m_size;
    if(!m_rootNode)
        return;
    m_rootNode = m_rootNode->eraceNode(key);
}

template<typename Val, uint KeyLen>
void Tree<Val, KeyLen>::clear()
{
    while (m_rootNode != nullptr) {
        erase(m_rootNode->m_key);
    }
}

#endif // SEARCHTREE_H
