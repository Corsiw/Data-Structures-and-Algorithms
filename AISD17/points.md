# Точки сочленения

## Наивный алгоритм
Для каждой вершины графа удаляем эту вершину и проверяем на достижимость всех оставшихся:

```c++
for v in vertex:
    markAsRemoved(v)
    int vis = 0
    if v == vertex[0]:
        vis = DFS(vertex[1])
    else:
        vis DFS(vertex[0])
        
    if vis < v.length() - 1:
        res.push_back(v)
    unmark(v)
```

n = |V|,
m = |E|

Сложность DFS - O(n + m)

Сложность нашего алгоритма - O(n * (n + m))

## Алгоритм Тарьяна
Мы делаем один DFS и для каждой вершины считаем:  
disc[v] — время входа в вершину (номер шага DFS)  
low[v] — минимальное время входа вершины, достижимой.

Если у вершины v существует ребенок, такой что low[child] >= disc[v],
то из ребенка нельзя попасть выше v и если удалить v, то поддерево child отсоединится.

Особый случай корень DFS:
Корень является точкой сочленения, если у него больше одного ребёнка в DFS.

```c++
vector<int> disc, low;
vector<bool> visited;
set<int> articulation_points;
int timer = 0;

void dfs(int v, int parent = -1) {
    visited[v] = true;
    disc[v] = low[v] = timer++;
    int children = 0;

    for (int to : adj[v]) {
        if (to == parent) condiscue;

        if (visited[to]) {
            // обратное ребро
            low[v] = min(low[v], disc[to]);
        } else {
            dfs(to, v);
            low[v] = min(low[v], low[to]);

            if (low[to] >= disc[v] && parent != -1)
                articulation_points.insert(v);

            children++;
        }
    }

    if (parent == -1 && children > 1)
        articulation_points.insert(v);
}

/// В мейн нужно запустить алгоритм для каждой непосещеннной вершины 
```