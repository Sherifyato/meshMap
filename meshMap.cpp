#include <bits/stdc++.h>

using namespace std;

void yato() {
    ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("../in.txt", "r", stdin);
    freopen("../out.txt", "w", stdout);
#endif
}

class meshHap {
private:
    deque<int> *ptrArray;
    int sizeComponent, noAllArrays, totalSize, maxSize, noUsedArrays;

    [[nodiscard]] pair<int, int> position(int idx) const {
        int component = idx / sizeComponent;
        int index = idx % sizeComponent;
        return {component, index};
    }

    int *ptr(int idx) {
        pair<int, int> pos = position(idx);
        int component = pos.first, index = pos.second;
        return &ptrArray[component][index];
    }

    void updateComponentSize() {
        if (totalSize < maxSize)
            return;

        int newMaxSize = 2 * maxSize;
        int newSizeComponent = ceil(sqrt(newMaxSize));
        int newNoArrays = newSizeComponent;
        deque<int> *temp;
        temp = new deque<int>[newNoArrays];

        int idx = 0;
        for (int i = 0, oldI = 0, oldJ = 0; i < newNoArrays; ++i) {
            temp[i].resize(newSizeComponent);

            for (int j = 0; j < newSizeComponent && idx < totalSize; ++j) {
                temp[i][j] = ptrArray[oldI][oldJ];
                oldJ++;
                if (oldJ == ptrArray[oldI].size()) {
                    oldJ = 0;
                    oldI++;
                }
                idx++;
            }
        }
        swap(ptrArray, temp);
        sizeComponent = newSizeComponent;
        noUsedArrays = newNoArrays;
        noAllArrays = newNoArrays;
        maxSize = newMaxSize;
        delete[] temp;
    }

    void outOfBounds(int idx, bool insert = false) const {
        if (idx > (totalSize - 1) + insert || idx < 0)
            throw std::out_of_range("Index is out of bounds.");
    }

    void updateSizeInfo(int add) {
        totalSize += add;
        noUsedArrays = (totalSize + sizeComponent - 1) / sizeComponent;
    }

public:

    explicit meshHap(vector<int> &arr) {
        totalSize = arr.size();
        sizeComponent = ceil(sqrt(totalSize));
        noAllArrays = 2 * sizeComponent;
        maxSize = noAllArrays * sizeComponent;
        ptrArray = new deque<int>[noAllArrays];
        noUsedArrays = (totalSize + sizeComponent - 1) / sizeComponent;
        int idx = 0;

        for (int i = 0; i < noAllArrays; ++i) {
            ptrArray[i].resize(sizeComponent);

            for (int j = 0; j < sizeComponent && idx < totalSize; ++j) {
                ptrArray[i][j] = arr[idx++];
            }
        }
    }

    meshHap(int size = 0) {
        totalSize = size;
        sizeComponent = ceil(sqrt(totalSize));
        sizeComponent = max(sizeComponent, 1);
        noAllArrays = 2 * sizeComponent;
        maxSize = noAllArrays * sizeComponent;
        ptrArray = new deque<int>[noAllArrays];
        noUsedArrays = (totalSize + sizeComponent - 1) / sizeComponent;
        int idx = 0;

        for (int i = 0; i < noAllArrays; ++i) {
            ptrArray[i].resize(sizeComponent);

            for (int j = 0; j < sizeComponent && idx < totalSize; ++j) {
                ptrArray[i][j] = 0;
            }
        }
    }

    meshHap(const meshHap &other) {
        totalSize = other.totalSize;
        sizeComponent = other.sizeComponent;
        noAllArrays = other.noAllArrays;
        maxSize = other.maxSize;
        noUsedArrays = other.noUsedArrays;
        ptrArray = new deque<int>[noAllArrays];

        for (int i = 0; i < noAllArrays; ++i) {
            ptrArray[i].resize(sizeComponent);

            for (int j = 0; j < sizeComponent; ++j) {
                ptrArray[i][j] = other.ptrArray[i][j];
            }
        }

    }

    explicit meshHap(const int arr[]) {
        totalSize = sizeof(arr) / sizeof(arr[0]);
        sizeComponent = ceil(sqrt(totalSize));
        noAllArrays = 2 * sizeComponent;
        maxSize = noAllArrays * sizeComponent;
        ptrArray = new deque<int>[noAllArrays];

        int idx = 0;
        noUsedArrays = (totalSize + sizeComponent - 1) / sizeComponent;

        for (int i = 0; i < noAllArrays; ++i) {
            ptrArray[i].resize(sizeComponent);

            for (int j = 0; j < sizeComponent && idx < totalSize; ++j) {
                ptrArray[i][j] = arr[idx++];
            }

        }

    }

    [[nodiscard]] unsigned size() const {
        return totalSize;
    }

    [[nodiscard]] bool isFull() const {
        return totalSize == maxSize;
    }

    [[nodiscard]] bool isEmpty() const {
        return totalSize == 0;
    }

    int &operator[](int idx) {
        outOfBounds(idx);

        return *ptr(idx);
    }

    void updateValue(int idx, int val) {
        outOfBounds(idx);

        *ptr(idx) = val;
    }


    void deleteAt(int idx) {

        outOfBounds(idx);

        pair<int, int> pos = position(idx);
        int component = pos.first, index = pos.second;

        ptrArray[component].erase(ptrArray[component].begin() + index);
        for (int i = component; i < noUsedArrays - 1; ++i) {
            ptrArray[i].push_back(ptrArray[i + 1].front());
            ptrArray[i + 1].pop_front();
        }
        ptrArray[noUsedArrays - 1].push_back(0);

        updateSizeInfo(-1);
    }

    void insertAt(int idx, int val) {

        outOfBounds(idx, true);
        updateComponentSize();

        pair<int, int> pos = position(idx);
        int component = pos.first, index = pos.second;

        // some edge cases
        int elementsInLastComponent = totalSize - (noUsedArrays - 1) * sizeComponent;
        if (component == noUsedArrays - 1 && elementsInLastComponent < sizeComponent) {
            for (int i = elementsInLastComponent; i > index; --i) {
                ptrArray[component][i] = ptrArray[component][i - 1];
            }
            ptrArray[component][index] = val;
            updateSizeInfo(1);
            return;
        } else if (component == noUsedArrays) {
            ptrArray[component].pop_back();
            ptrArray[component].push_front(val);
            updateSizeInfo(1);
            return;
        }

        vector<deque<int> *> temp;
        temp.resize(noAllArrays);
        for (int i = 0; i < noAllArrays; ++i) {
            temp[i] = &ptrArray[i];
        }

        ptrArray[component].insert(ptrArray[component].begin() + index, val);
        for (int i = component + 1; i < noUsedArrays; ++i) {
            ptrArray[i].push_front(ptrArray[i - 1].back());
            ptrArray[i - 1].pop_back();
        }
        if (noUsedArrays != noAllArrays) {
            ptrArray[noUsedArrays].push_front(ptrArray[noUsedArrays - 1].back());
            ptrArray[noUsedArrays - 1].pop_back();
            ptrArray[noUsedArrays].pop_back();
        } else {
            ptrArray[noUsedArrays - 1].pop_back();
        }

        updateSizeInfo(1);
    }

    bool operator==(const meshHap &other) const {

        bool areEqual = other.totalSize == this->totalSize;
        if (!areEqual) return false;

        for (int i = 0; i < noUsedArrays; ++i) {
            for (int j = 0; i * sizeComponent + j < totalSize; ++j) {

                if (ptrArray[i][j] != other.ptrArray[i][j]) {
                    areEqual = false;
                    break;
                }

            }
        }

        return areEqual;
    }

    bool isSorted() {

        for (int i = 0; i < totalSize - 1; ++i) {
            if (*this->ptr(i) > *this->ptr(i + 1)) {
                return false;
            }
        }
        return true;

    }

    ~meshHap() {
        delete[] ptrArray;
    }
};

int main() {
    yato();

}