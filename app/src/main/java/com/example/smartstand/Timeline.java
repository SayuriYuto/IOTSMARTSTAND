package com.example.smartstand;

import android.app.ProgressDialog;
import android.os.Bundle;
import android.util.Log;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.google.firebase.firestore.DocumentChange;
import com.google.firebase.firestore.EventListener;
import com.google.firebase.firestore.FirebaseFirestore;
import com.google.firebase.firestore.FirebaseFirestoreException;
import com.google.firebase.firestore.ListenerRegistration;
import com.google.firebase.firestore.Query;
import com.google.firebase.firestore.QuerySnapshot;

import java.util.ArrayList;

import Adapter.HistoryAdapter;
import Adapter.TimeLineAdapter;
import Model.HistoryModel;
import Model.TimeLineModel;

public class Timeline extends AppCompatActivity {
    private RecyclerView recyclerView;
    private ArrayList<TimeLineModel> historyList;
    private TimeLineAdapter historyAdapter;
    private FirebaseFirestore db;
    private Query query;
    private ListenerRegistration listenerRegistration;
    private ProgressDialog progressDialog;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.timeline);
        progressDialog = new ProgressDialog(this);
        progressDialog.setCancelable(false);
        progressDialog.setMessage("Fetching Data.");
        recyclerView = findViewById(R.id.recycerlview);
        db = FirebaseFirestore.getInstance();
        progressDialog.show();
        recyclerView.setHasFixedSize(true);
        recyclerView.setLayoutManager(new LinearLayoutManager(Timeline.this));
        historyList = new ArrayList<>();
        historyAdapter = new TimeLineAdapter(Timeline.this, historyList);
        recyclerView.setAdapter(historyAdapter);
        EventChangeListener();
    }
        private void EventChangeListener() {
            db.collection("timeline")
                    .addSnapshotListener(new EventListener<QuerySnapshot>() {
                        @Override
                        public void onEvent(@Nullable QuerySnapshot value, @Nullable FirebaseFirestoreException error) {
                            if(error!= null){
                                if(progressDialog.isShowing())
                                    progressDialog.dismiss();
                                Log.e("Firestore Error",error.getMessage());
                                return;
                            }
                            for (DocumentChange dc : value.getDocumentChanges()) {
                                if (dc.getType() == DocumentChange.Type.ADDED) {
                                    historyList.add(dc.getDocument().toObject(TimeLineModel.class));
                                    historyAdapter.notifyDataSetChanged();
                                }
                                historyAdapter.notifyDataSetChanged();
                            }
                            if (progressDialog.isShowing())
                                progressDialog.dismiss();
                        }
                    });
        }
    }

