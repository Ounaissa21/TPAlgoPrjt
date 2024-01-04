#include <gtk/gtk.h>

// fonction pour afficher les valeurs du tableau dans les labels
void update_labels(GtkWidget *labels[], int arr[], int n) {
    for (int i = 0; i < n; i++) {
        gchar *text = g_strdup_printf("%d", arr[i]);
        gtk_label_set_text(GTK_LABEL(labels[i]), text);
        g_free(text);
    }
}

// fonction pour mettre à jour les labels du tableau en modifiant les valeurs du tableau arr[]
void update_table(int arr[], int n) {
    // mettre à jour les valeurs du tableau ici
    // ...
}

// fonction pour simuler le tri par fusion
void simulate_merge_sort(GtkWidget *labels[], int arr[], int n) {
    update_labels(labels, arr, n);

    // simuler le tri par fusion ici
    // ...
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *labels[10];
    int arr[10] = {10, 7, 8, 9, 1, 5, 2, 3, 6, 4};

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Tri par fusion");
    gtk_window_set_default_size (GTK_WINDOW (window), 300, 200);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_window_set_child (GTK_WINDOW (window), grid);

    for (int i = 0; i < 10; i++) {
        labels[i] = gtk_label_new("");
        gtk_grid_attach(GTK_GRID(grid), labels[i], i, 0, 1, 1);
    }

    update_labels(labels, arr, 10);

    gtk_widget_show_all(window);

    simulate_merge_sort(labels, arr, 10);
}

int
main (int    argc,
      char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}