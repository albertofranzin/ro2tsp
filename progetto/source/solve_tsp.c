#include "graph.h"
#include "solve_tsp.h"

graph INITIAL_GRAPH;

int calls = 0;

void solve_tsp(graph* G, graph* F, graph* H, double* incumbent, int flag) {
    int i, j, u, v, w;
    double z;
    graph ONE_TREE;
    int n = (*G).n;
    double* previous_cost;
    double cost_wv, cost_wu;

    calls += 1;
    initGraph(&ONE_TREE, 1);

    /* effettua una copia del grafo iniziale G, passato in ingresso alla prima chiamata della funzione;
     */
    if (flag == 0) {
        initGraph(&INITIAL_GRAPH, 1);
        copyGraph(G, &INITIAL_GRAPH);

        // avoid initial computation of the 1-tree, by using the one
        // computed by the subgradient algorithm
        copyGraph(F, &ONE_TREE);

        flag = 1;
    }
    else {
        flag = 2;
        /* calcola 1-albero;
         */
        //compute_ot(G, &ONE_TREE);

        // avoid initial computation of the 1-tree, by using the one
        // computed by caller
        copyGraph(F, &ONE_TREE);
    }

    /*if (is_cycle(&ONE_TREE)) {
        plotGraph(F, &ONE_TREE, "onetree", NULL);
    }*/

    /* calcola z = costo dell'1-albero;
     */
    for (i = 1; i <= n; i++) {
        for (j = i+1; j <= n; j++)
            set_edge_cost(&ONE_TREE, i, j, get_edge_cost(&INITIAL_GRAPH, i, j));
    }
    z = get_graph_cost(&ONE_TREE);
    //printf("current z = %f\n", z);

    /* verifica se possibile potare il ramo corrente
     */
    if (z >= *incumbent) {
        deleteGraph(&ONE_TREE);
        return;
    }

    /* verifica se è stato trovato un ciclo (una soluzione ottima per il sottoproblema corrispondente al nodo corrente);
     * aggiorna incumbent e soluzione ottima corrente;
     */
    if (is_cycle(&ONE_TREE)) {
        printf("updating incumbent : from %f to %f\n", *incumbent, z);
        printf("calls so far: %d\n", calls);
        *incumbent = z;
        copyGraph(&ONE_TREE, H);

        plotGraph(&ONE_TREE, H, "default", NULL);

        deleteGraph(&ONE_TREE);
        return;
    }

    /* cerca un nodo w con almeno 3 lati incidenti;
     */
    for (w = 1; w <= n; w++) {
        if (get_node_deg(&ONE_TREE, w) >= 3)
            break;
    }

    node nl[n];
    memcpy(nl, (ONE_TREE.V), sizeof(node) * n);/**/

    /*for (i = 1; i < ONE_TREE.n; ++i) {
        printf("%d %d %d || %d %d\n", i, nl[i].name, nl[i].deg,
                G->V[i].name, G->V[i].deg);
    }/**/

    qsort((void *)&nl, n, sizeof(node), (compfn)snbdComp);

    /*for (i = 1; i < ONE_TREE.n; ++i) {
        printf("%d %d %d\n", i, nl[i].name, nl[i].deg);
    }/**/

    //char ch = getchar();

    // since we don't have a cycle, there is a node with deg = 1
    i = 1;
    u = v = 0;
    int tmpn1 = 0, tmpn2 = 0;/**/
    //double tmpc1, tmpc2;
    //while(i < n && nl[i].deg < 3) i++;
    //tmpn2 = i;
    //tmpc1 = get_edge_cost(&ONE_TREE, tmpn1+1, tmpn2+1);
    //double tmpc[nl[i].deg];

    /*printf("%d\n", i);
    /*while(i < n && nl[i].deg == 3) {
        i++;
        if (adjacent(&ONE_TREE, tmpn1+1, tmpn2+1)) {
            continue;
        }
        // choose the farthest node
        tmpc2 = get_edge_cost(&ONE_TREE, tmpn1+1, tmpn2+1);
        if (tmpc2 > tmpc1) {
        }
    }*/
    /*if (i < n) {
        // check which edge can be connected to tmpn1 instead of to tmpn2
        j = 0;
        while (j < n) {
            if (j != i && nl[j].name != nl[tmpn1-1].name && adjacent(&ONE_TREE, nl[tmpn2].name, nl[j+1].name) &&
                    get_edge_cost(&ONE_TREE, nl[tmpn1+1].name, nl[j+1].name) > SMALL &&
                    get_edge_cost(&ONE_TREE, nl[tmpn1+1].name, nl[j+1].name) < BIG) {
                if (get_edge_cost(&ONE_TREE, nl[tmpn1+1].name, nl[j+1].name) > tmpc1) {
                    tmpn2 = j;
                    tmpc1 = get_edge_cost(&ONE_TREE, nl[tmpn1+1].name, nl[j+1].name);
                    // printf("%d %d %f\n", tmpn1+1, tmpn2+2, tmpc1);
                }
            }
            j++;
        }
    }*/

    //w = nl[tmpn2].name;
    //w = nl[n-1].name;
    // v = nl[tmpn1].name;

    /* (tentativo di) ricerca di due lati candidati mai forzati nè vietati;
     */
    for (v = 1; v <= n; v++) {
        if (v != w && adjacent(&ONE_TREE, w, v) && get_edge_cost(G, w, v) > SMALL && get_edge_cost(G, w, v) < BIG)
            break;
    }
    for (u = v+1; u <= n; u++) {
        if (u != w && u != v && adjacent(&ONE_TREE, w, u) && get_edge_cost(G, w, u) > SMALL && get_edge_cost(G, w, u) < BIG)
            break;
    }/**/

    /*int wd = ONE_TREE.V[w].deg, dest[n], mark = 0;
    double c[n], ctmp;
    memset(c, 0, sizeof(c));
    for (v = 1; v <= n; ++v) {
        ctmp = get_edge_cost(G, w, v);
        if (v != w && ctmp > SMALL && ctmp < BIG) {
            c[mark] = ctmp;
            dest[mark] = v;
            mark++;
        }
    }
    u = 0; v = 0;
    //printf("mark = %d\n", mark);
    if (mark == 0) {
        // no branch is possible
        return;
    }
    if (mark == 1) {
        v = dest[0];
        u = n+1;
    } else if (mark == 2) {
        if (c[0] > c[1]) {
            v = dest[0];
            u = dest[1];
        } else {
            u = dest[0];
            v = dest[1];
        }
    } else  {//if (mark == wd) {
        // get max
        // get second max
        u = dest[0];
        v = dest[1];
    }*/

    //printf("%d - %d %d | %d\n", w, v,u,n);/**/

    deleteGraph(&ONE_TREE);

    /* se esistono due lati mai selezionati allora procedi con BRANCHING A 3 VIE;
     */
    if ((v >= 1 && v <= n) && (u >= 1 && u <= n)) {

        /*
         * forward checking of the best branch on the node
         */

        graph //FC_ONE_TREE,
              FC_ONE_TREE1,
              FC_ONE_TREE2,
              FC_ONE_TREE3;
        //initGraph(&FC_ONE_TREE, 1);
        initGraph(&FC_ONE_TREE1, 1);
        initGraph(&FC_ONE_TREE2, 1);
        initGraph(&FC_ONE_TREE3, 1);


        double z1, z2, z3;

        // 1 : deny wv
        cost_wv = get_edge_cost(G, w, v);
        set_edge_cost(G, w, v, BIG);

        compute_ot(G, &FC_ONE_TREE1);

        // restore original cost, in both graphs
        set_edge_cost(&FC_ONE_TREE1, w, v, cost_wv);
        set_edge_cost(G, w, v, cost_wv);

        z1 = get_graph_cost(&FC_ONE_TREE1);

        // 2 : impose wv, deny wu
        cost_wv = get_edge_cost(G, w, v);
        cost_wu = get_edge_cost(G, w, u);
        set_edge_cost(G, w, v, SMALL);
        set_edge_cost(G, w, u, BIG);

        compute_ot(G, &FC_ONE_TREE2);

        // restore original costs, in both graphs
        set_edge_cost(&FC_ONE_TREE2, w, v, cost_wv);
        set_edge_cost(&FC_ONE_TREE2, w, u, cost_wu);
        set_edge_cost(G, w, v, cost_wv);
        set_edge_cost(G, w, u, cost_wu);

        z2 = get_graph_cost(&FC_ONE_TREE2);

        // 3 impose wu, wv, deny all the others
        previous_cost = malloc(sizeof(double) * n);
        for (i = 1; i <= n; i++) { // memorizza i costi attuali dei lati per poter fare un roll back al ritorno dalla prossim chiamata ricorsiva;
            if (i != w) 
                previous_cost[i-1] = get_edge_cost(G, w, i);
        }
        set_edge_cost(G, w, v, SMALL);
        set_edge_cost(G, w, u, SMALL);
        for (i = 1; i <= n; i++) {
            if (i != w && i != v && i != u)
                set_edge_cost(G, w, i, BIG);
        }

        compute_ot(G, &FC_ONE_TREE3);

        // restore original costs, in both graphs
        for (i = 1; i <= n; i++) { // roll back dei costi così com'erano prima della chiamata ricorsiva;
            if (i != w)
                set_edge_cost(&FC_ONE_TREE3, w, i, previous_cost[i-1]);
        }
        set_edge_cost(&FC_ONE_TREE3, w, v, SMALL);
        set_edge_cost(&FC_ONE_TREE3, w, u, SMALL);
        for (i = 1; i <= n; i++) { // roll back dei costi così com'erano prima della chiamata ricorsiva;
            if (i != w)
                set_edge_cost(G, w, i, previous_cost[i-1]);
        }
        free(previous_cost);

        z3 = get_graph_cost(&FC_ONE_TREE3);

        // branch on best result
        // fa un po' cagare perché non riuso niente di quanto ho calcolato prima,
        // ma per ora proviamo così

        if (z1 >= z2 && z2 >= z3) {

            /*
             * The three forward checks rank this way:
             * - denying edge {w,v} is the most promising branch
             *       (the one with the immediate higher bound)
             * - impose edge {w,v}, deny {w,u}
             * - impose {w,v}, {w,u}, deny the rest
             *       is the route that gives the worst (immediate) bound improve
             */

            if (z1 < *incumbent) {
                // vieta il lato {w, v};
                cost_wv = get_edge_cost(G, w, v);
                set_edge_cost(G, w, v, BIG);
                solve_tsp(G, &FC_ONE_TREE1, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
            }

            if (z2 < *incumbent) {
                // forza il lato {w, v} e vieta il lato {w, u};
                cost_wv = get_edge_cost(G, w, v);
                cost_wu = get_edge_cost(G, w, u);
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, BIG);
                solve_tsp(G, &FC_ONE_TREE2, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
                set_edge_cost(G, w, u, cost_wu);
            }

            if (z3 < *incumbent) {
                // forza i lati {w, v} e {w, u}, vieta tutti gli altri lati;
                previous_cost = (double*)malloc(sizeof(double) * n);
                for (i = 1; i <= n; i++) { // memorizza i costi attuali dei lati per poter fare un roll back al ritorno dalla prossim chiamata ricorsiva;
                    if (i != w) 
                        previous_cost[i-1] = get_edge_cost(G, w, i);
                }
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, SMALL);
                for (i = 1; i <= n; i++) {
                    if (i != w && i != v && i != u)
                        set_edge_cost(G, w, i, BIG);
                }
                solve_tsp(G, &FC_ONE_TREE3, H, incumbent, flag);
                for (i = 1; i <= n; i++) { // roll back dei costi così com'erano prima della chiamata ricorsiva;
                    if (i != w)
                        set_edge_cost(G, w, i, previous_cost[i-1]);
                }
                free(previous_cost);
            }

            // endif z1 >= z2 && z2 >= z3

        } else if (z3 >= z2 && z2 >= z1) {

            /*
             * The three forward checks rank this way:
             * - impose {w,v}, {w,u}, deny the rest
             *       gives the immediate higher bound
             * - impose edge {w,v}, deny {w,u}
             * - denying edge {w,v} is the least promising branch
             *       is the route that gives the worst (immediate) bound improve
             */

            if (z3 < *incumbent) {
                // forza i lati {w, v} e {w, u}, vieta tutti gli altri lati;
                previous_cost = (double*)malloc(sizeof(double) * n);
                for (i = 1; i <= n; i++) { // memorizza i costi attuali dei lati per poter fare un roll back al ritorno dalla prossim chiamata ricorsiva;
                    if (i != w) 
                        previous_cost[i-1] = get_edge_cost(G, w, i);
                }
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, SMALL);
                for (i = 1; i <= n; i++) {
                    if (i != w && i != v && i != u)
                        set_edge_cost(G, w, i, BIG);
                }
                solve_tsp(G, &FC_ONE_TREE3, H, incumbent, flag);
                for (i = 1; i <= n; i++) { // roll back dei costi così com'erano prima della chiamata ricorsiva;
                    if (i != w)
                        set_edge_cost(G, w, i, previous_cost[i-1]);
                }
                free(previous_cost);
            }

            if (z2 < *incumbent) {
                // forza il lato {w, v} e vieta il lato {w, u};
                cost_wv = get_edge_cost(G, w, v);
                cost_wu = get_edge_cost(G, w, u);
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, BIG);
                solve_tsp(G, &FC_ONE_TREE2, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
                set_edge_cost(G, w, u, cost_wu);
            }

            if (z1 < *incumbent) {
                // vieta il lato {w, v};
                cost_wv = get_edge_cost(G, w, v);
                set_edge_cost(G, w, v, BIG);
                solve_tsp(G, &FC_ONE_TREE1, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
            }

        } else if (z3 >= z1 && z1 >= z2) {

            /*
             * The three forward checks rank this way:
             * - denying edge {w,v} is the most promising branch
             *       (the one with the immediate higher bound)
             * - impose {w,v}, {w,u}, deny the rest
             * - impose edge {w,v}, deny {w,u}
             *       is the route that diges the worst (immediate) bound improve
             */

            if (z3 < *incumbent) {
                // forza i lati {w, v} e {w, u}, vieta tutti gli altri lati;
                previous_cost = (double*)malloc(sizeof(double) * n);
                for (i = 1; i <= n; i++) { // memorizza i costi attuali dei lati per poter fare un roll back al ritorno dalla prossim chiamata ricorsiva;
                    if (i != w) 
                        previous_cost[i-1] = get_edge_cost(G, w, i);
                }
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, SMALL);
                for (i = 1; i <= n; i++) {
                    if (i != w && i != v && i != u)
                        set_edge_cost(G, w, i, BIG);
                }
                solve_tsp(G, &FC_ONE_TREE3, H, incumbent, flag);
                for (i = 1; i <= n; i++) { // roll back dei costi così com'erano prima della chiamata ricorsiva;
                    if (i != w)
                        set_edge_cost(G, w, i, previous_cost[i-1]);
                }
                free(previous_cost);
            }

            if (z1 < *incumbent) {
                // vieta il lato {w, v};
                cost_wv = get_edge_cost(G, w, v);
                set_edge_cost(G, w, v, BIG);
                solve_tsp(G, &FC_ONE_TREE1, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
            }

            if (z2 < *incumbent) {
                // forza il lato {w, v} e vieta il lato {w, u};
                cost_wv = get_edge_cost(G, w, v);
                cost_wu = get_edge_cost(G, w, u);
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, BIG);
                solve_tsp(G, &FC_ONE_TREE2, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
                set_edge_cost(G, w, u, cost_wu);
            }

        } else if (z1 >= z3 && z3 >= z2) {

            /*
             * The three forward checks rank this way:
             * - denying edge {w,v} is the most promising branch
             *       (the one with the immediate higher bound)
             * - impose {w,v}, {w,u}, deny the rest
             * - impose edge {w,v}, deny {w,u}
             *       is the route that diges the worst (immediate) bound improve
             */

            if (z1 < *incumbent) {
                // vieta il lato {w, v};
                cost_wv = get_edge_cost(G, w, v);
                set_edge_cost(G, w, v, BIG);
                solve_tsp(G, &FC_ONE_TREE1, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
            }

            if (z3 < *incumbent) {
                // forza i lati {w, v} e {w, u}, vieta tutti gli altri lati;
                previous_cost = (double*)malloc(sizeof(double) * n);
                for (i = 1; i <= n; i++) { // memorizza i costi attuali dei lati per poter fare un roll back al ritorno dalla prossim chiamata ricorsiva;
                    if (i != w) 
                        previous_cost[i-1] = get_edge_cost(G, w, i);
                }
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, SMALL);
                for (i = 1; i <= n; i++) {
                    if (i != w && i != v && i != u)
                        set_edge_cost(G, w, i, BIG);
                }
                solve_tsp(G, &FC_ONE_TREE3, H, incumbent, flag);
                for (i = 1; i <= n; i++) { // roll back dei costi così com'erano prima della chiamata ricorsiva;
                    if (i != w)
                        set_edge_cost(G, w, i, previous_cost[i-1]);
                }
                free(previous_cost);
            }

            if (z2 < *incumbent) {
                // forza il lato {w, v} e vieta il lato {w, u};
                cost_wv = get_edge_cost(G, w, v);
                cost_wu = get_edge_cost(G, w, u);
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, BIG);
                solve_tsp(G, &FC_ONE_TREE2, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
                set_edge_cost(G, w, u, cost_wu);
            }

        } else if (z2 >= z3 && z3 >= z1) {

            if (z2 < *incumbent) {
                // forza il lato {w, v} e vieta il lato {w, u};
                cost_wv = get_edge_cost(G, w, v);
                cost_wu = get_edge_cost(G, w, u);
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, BIG);
                solve_tsp(G, &FC_ONE_TREE2, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
                set_edge_cost(G, w, u, cost_wu);
            }

            if (z3 < *incumbent) {
                // forza i lati {w, v} e {w, u}, vieta tutti gli altri lati;
                previous_cost = (double*)malloc(sizeof(double) * n);
                for (i = 1; i <= n; i++) { // memorizza i costi attuali dei lati per poter fare un roll back al ritorno dalla prossim chiamata ricorsiva;
                    if (i != w) 
                        previous_cost[i-1] = get_edge_cost(G, w, i);
                }
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, SMALL);
                for (i = 1; i <= n; i++) {
                    if (i != w && i != v && i != u)
                        set_edge_cost(G, w, i, BIG);
                }
                solve_tsp(G, &FC_ONE_TREE3, H, incumbent, flag);
                for (i = 1; i <= n; i++) { // roll back dei costi così com'erano prima della chiamata ricorsiva;
                    if (i != w)
                        set_edge_cost(G, w, i, previous_cost[i-1]);
                }
                free(previous_cost);
            }

            if (z1 < *incumbent) {
                // vieta il lato {w, v};
                cost_wv = get_edge_cost(G, w, v);
                set_edge_cost(G, w, v, BIG);
                solve_tsp(G, &FC_ONE_TREE1, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
            }

        } else {

            if (z2 < *incumbent) {
                // forza il lato {w, v} e vieta il lato {w, u};
                cost_wv = get_edge_cost(G, w, v);
                cost_wu = get_edge_cost(G, w, u);
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, BIG);
                solve_tsp(G, &FC_ONE_TREE2, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
                set_edge_cost(G, w, u, cost_wu);
            }

            if (z1 < *incumbent) {
                // vieta il lato {w, v};
                cost_wv = get_edge_cost(G, w, v);
                set_edge_cost(G, w, v, BIG);
                solve_tsp(G, &FC_ONE_TREE1, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
            }

            if (z3 < *incumbent) {
                // forza i lati {w, v} e {w, u}, vieta tutti gli altri lati;
                previous_cost = (double*)malloc(sizeof(double) * n);
                for (i = 1; i <= n; i++) { // memorizza i costi attuali dei lati per poter fare un roll back al ritorno dalla prossim chiamata ricorsiva;
                    if (i != w) 
                        previous_cost[i-1] = get_edge_cost(G, w, i);
                }
                set_edge_cost(G, w, v, SMALL);
                set_edge_cost(G, w, u, SMALL);
                for (i = 1; i <= n; i++) {
                    if (i != w && i != v && i != u)
                        set_edge_cost(G, w, i, BIG);
                }
                solve_tsp(G, &FC_ONE_TREE3, H, incumbent, flag);
                for (i = 1; i <= n; i++) { // roll back dei costi così com'erano prima della chiamata ricorsiva;
                    if (i != w)
                        set_edge_cost(G, w, i, previous_cost[i-1]);
                }
                free(previous_cost);
            }

        }

        deleteGraph(&FC_ONE_TREE1);
        deleteGraph(&FC_ONE_TREE2);
        deleteGraph(&FC_ONE_TREE3);

    }

    /* esiste un solo lato che non è mai stato nè forzato nè vietato;
     */
    else if (((v >= 1 && v <= n) && (u < 1 || u > n)) || ((v < 1 || v > n) && (u >= 1 && u <= n))) {

        graph FC_ONE_TREE1,
              FC_ONE_TREE2;
        initGraph(&FC_ONE_TREE1, 1);
        initGraph(&FC_ONE_TREE2, 1);

        double z1, z2;

        // vieta il lato {w, v};
        cost_wv = get_edge_cost(G, w, v);
        set_edge_cost(G, w, v, BIG);
        compute_ot(G, &FC_ONE_TREE1);
        set_edge_cost(G, w, v, cost_wv);
        set_edge_cost(&FC_ONE_TREE1, w, v, cost_wv);

        z1 = get_graph_cost(&FC_ONE_TREE1);

        // solve_tsp(G, H, incumbent, flag);

        // forza il lato {w, v};
        cost_wv = get_edge_cost(G, w, v);
        set_edge_cost(G, w, v, SMALL);
        compute_ot(G, &FC_ONE_TREE2);
        set_edge_cost(G, w, v, cost_wv);
        set_edge_cost(&FC_ONE_TREE2, w, v, cost_wv);

        z2 = get_graph_cost(&FC_ONE_TREE2);

        if (z1 >= z2) {

            /*
             * to deny edge {w,v} gives an (immediate) better bound
             * than imposing it : let's try this way first
             */

            if (z1 < *incumbent) {
                // vieta il lato {w, v};
                cost_wv = get_edge_cost(G, w, v);
                set_edge_cost(G, w, v, BIG);
                solve_tsp(G, &FC_ONE_TREE1, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
            }

            if (z2 < *incumbent) {
                // forza il lato {w, u};
                cost_wv = get_edge_cost(G, w, v);
                set_edge_cost(G, w, v, SMALL);
                solve_tsp(G, &FC_ONE_TREE2, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
            }

        } else {

            /*
             * to impose edge {w,v} gives an (immediate) better bound
             * than denying it : let's try this way first
             */

            if (z2 < *incumbent) {
                // forza il lato {w, u};
                cost_wv = get_edge_cost(G, w, v);
                set_edge_cost(G, w, v, SMALL);
                solve_tsp(G, &FC_ONE_TREE2, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
            }

            if (z1 < *incumbent) {
                // vieta il lato {w, v};
                cost_wv = get_edge_cost(G, w, v);
                set_edge_cost(G, w, v, BIG);
                solve_tsp(G, &FC_ONE_TREE1, H, incumbent, flag);
                set_edge_cost(G, w, v, cost_wv);
            }

        }

        deleteGraph(&FC_ONE_TREE1);
        deleteGraph(&FC_ONE_TREE2);

    }

    /* tutti i lati sono già stati forzati o vietati;
     */
    else if ((v < 1 || v > n) && (u < 1 || u > n)) {
        return;
    }

    /* copia il grafo iniziale nuovamente il G (la funzione in questo modo non modifica gli oggetti passati in ingresso);
     */
    if (flag == 1) {
        copyGraph(&INITIAL_GRAPH, G);
        deleteGraph(&INITIAL_GRAPH);
        flag = 2;
    }

    //printf("-- %d\n", calls);
}

/* ritorna 1 se G è un ciclo, 0 altrimenti;
 * - si assume G connesso (altrimenti non è detto che G sia un ciclo anche se la funzione ritorna 1;
 */
int is_cycle(graph* G) {
    int i;
    int n = (*G).n;
 
    for (i = 1; i <= n; i++) {
        if (get_node_deg(G, i) != 2)
            return 0;
    }
    return 1;
}
