#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#define N 50

int taille_tableau;
int tableau[N];
GtkWidget *labels[N];

void merge(int tab[], int i1, int j1, int i2, int j2);
void tri_fusion(int tab[], int debut, int fin);

void creer_interface(int nb) {
    GtkWidget *window, *vbox, *button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Visualisation du tri par fusion");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    for (int i = 0; i < nb; i++) {
        labels[i] = gtk_label_new(" ");
        gtk_widget_set_size_request(labels[i], 40, 20);
        gtk_box_pack_start(GTK_BOX(vbox), labels[i], FALSE, FALSE, 5);
    }

    button = gtk_button_new_with_label("Tri");
    g_signal_connect(button, "clicked", G_CALLBACK(tri_fusion), (gpointer)nb);
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 5);

    gtk_widget_show_all(window);
}

void affiche_tableau(int tab[], int debut, int fin) {
    for (int i = debut; i <= fin; i++) {
        char ch[20];
        sprintf(ch, "%d", tab[i]);
        gtk_label_set_text(GTK_LABEL(labels[i]), ch);
    }
    gtk_widget_show_all(GTK_WIDGET(labels[0]));
}

void tri_fusion(int tab[], int debut, int fin) {
    int i, j, k, mid;

    if (debut < fin) {
        mid = (debut + fin) / 2;

        tri_fusion(tab, debut, mid);
        tri_fusion(tab, mid + 1, fin);

        merge(tab, debut, mid, mid + 1, fin);
    }
}

void merge(int tab[], int i1, int j1, int i2, int j2) {
    int tab1[j1 - i1 + 1], tab2[j2 - i2 + 1];
    int i, j, k;

    for (i = i1; i <= j1; i++)
        tab1[i - i1] = tab[i];
    for (j = i2; j <= j2; j++)
        tab2[j - i2] = tab[j];

    i = 0;
    j = 0;
    k = i1;

    while (i <= j1 - i1 && j <= j2 - i2) {
        if (tab1[i] <= tab2[j]) {
            tab[k] = tab1[i];
            i++;
        } else {
            tab[k] = tab2[j];
            j++;
        }
        k++;
    }

    while (i <= j1 - i1) {
        tab[k] = tab1[i];
        i++;
        k++;
    }

    while (j <= j2 - i2) {
        tab[k] =0; j++; k++; }

        affiche_tableau(tab, i1, k - 1);
     }

int main(int argc, char *argv[]) {
     GtkWidget *window;   
     gtk_init(&argc, &argv);

taille_tableau = N;

for (int i = 0; i < N; i++) {
    tableau[i] = rand() % N;
}

creer_interface(N);

for (int i = 0; i < N; i++) {
    char ch[20];
    sprintf(ch, "%d", tableau[i]);
    gtk_label_set_text(GTK_LABEL(labels[i]), ch);
}

gtk_widget_show_all(GTK_WIDGET(labels[0]));

gtk_main();

return 0;
}