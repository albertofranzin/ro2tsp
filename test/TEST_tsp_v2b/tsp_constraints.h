#ifndef TSP_CONSTRAINTS_H_
#define TSP_CONSTRAINTS_H_

#include "constants.h"
#include "graph.h"
#include "elist.h"
#include "vector.h"
#include "lvector.h"
#include "tsp_status.h"
#include "tsp_backup.h"

// a=apply, p=propagate, c=check, ce=check enanched

// applica vincolo a {w, v} (ed eventualmente a  {w, u} se u!=0)
void tsp_constraints_a(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);
// applica vincolo a {w, v} (ed eventualmente a  {w, u} se u!=0) e propaga il vincolo ai nodi estremi del lato
void tsp_constraints_ap_simple(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);
// applica vincolo a {w, v} (ed eventualmente a  {w, u} se u!=0), propaga il vincolo ai nodi estremi del lato, e se possibile procedi ricorsivamente con la propagazione
void tsp_constraints_ap_recursive(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);
// come tsp_constraints_ap_recursive, ma l'implementazione è in forma ricorsiva e se ci sono nuovi vincoli trovati tramite propagazione, questi vengono applicati TUTTI subito, poi per ciascuno si controlla se possibile effettuare una nuova propagazione
void tsp_constraints_ap_iterative(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);

// applica vincolo a {w, v} (ed eventualmente a  {w, u} se u!=0) e controlla se i nuovi vincoli rendono impossibile una soluzione feasible: in particolare per ogni nodo #(lati forzati)<=2 e #(lati vietati)<=n-3
int tsp_constraints_ac(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);
// applica vincolo a {w, v} (ed eventualmente a  {w, u} se u!=0) e controlla se i nuovi vincoli rendono impossibile una soluzione feasible: in particolare per ogni nodo #(lati forzati)<=2, #(lati vietati)<=n-3 e nessun ciclo di lati forzati di lunghezza < n
int tsp_constraints_ace(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);

// combinazioni delle funzioni precedenti: e.g. tsp_constraints_acp_recursive effettua l'applicazione dei lati {w, v} e {w, u}, controlla se i vincli così aggiunti rendono impossibile una soluzione feasible, infine propaga ricorsivamente i vincoli così aggiunti
int tsp_constraints_acp_simple(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);
int tsp_constraints_acp_recursive(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);
int tsp_constraints_acp_iterative(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);

int tsp_constraints_acep_simple(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);
int tsp_constraints_acep_recursive(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);
int tsp_constraints_acep_iterative(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update);

#endif
