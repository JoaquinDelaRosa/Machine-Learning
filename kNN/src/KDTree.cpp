#include "KDTree.h"

#include "bits/stdc++.h"
#include "../../../Shared/include/DataSet/StatisticsManager.h"
#include "../../../Shared/include/DataSet/DataSet.h"
#include "../../../Shared/include/DataSet/Converter.h"
#include "Metric.h"

Converter* converter = new Converter();

KDTree::KDTree()
{
    this->marked = false;
    this->visited = false;
    this->feature = "";
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    this->entry = nullptr;
}

void KDTree::growHelper(DataSet* d, std::string target, int index){
    // Each leaf only has some threshold.
    this->entry = new DataSet(*d);
    this->target = target;

    if(d->getEntryCount() <= 1){
        this->label = 0;
        this->type = d->getFeatureAt(d->getFeatureIndex(target)).second;
        this->feature = target;
        return;
    }

    // Handle index
    if(d->getFeatureAt(index).first == target)
        index = (index + 1) % d->getFeatureCount();

    this->feature = d->getFeatureAt(index).first;
    this->type = d->getFeatureAt(index).second;

    // Make children
    this->left = new KDTree();
    this->right = new KDTree();
    left->parent = this;
    right->parent = this;

    // Make the partitions
    DataSet* l = new DataSet(*d, true);
    DataSet* r = new DataSet(*d, true);

    d->sortByFeature(this->feature);
    int N = d->getEntryCount();

    for(int i = 0; i < N; i++){
        if(i < N/2){
            l->addEntry(d->getEntryAt(i));
        }
        else if(i >= N/2){
            r->addEntry(d->getEntryAt(i));
        }
    }
    this->label = N/2;

    if(this->parent == nullptr){
        this->bounds.first = d->getEntryFeatureAt(0, this->feature);
        this->bounds.second = d->getEntryFeatureAt(N - 1, this->feature);
    }

    // Grow Children
    index = (index + 1) % d->getFeatureCount();
    left->bounds.first = d->getEntryFeatureAt(0, this->feature);
    left->bounds.second = d->getEntryFeatureAt(N/2 - 1, this->feature);
    right->bounds.first = d->getEntryFeatureAt(N/2 + 1, this->feature);
    right->bounds.second = d->getEntryFeatureAt(N - 1, this->feature);


    left->growHelper(l, target, index);
    right->growHelper(r, target, index);
}


void KDTree::grow(DataSet* d, std::string target){
    this->target = target;
    growHelper(d, target, 0);
}

KDTree* KDTree::traverse(std::map<std::string, std::string> query){
    if(this->left == nullptr && this->right == nullptr){
        if(this->visited == false && this->marked == false){
            this->visited = true;
            return this;
        }
    }

    std::string entry = query[this->feature];
    if(entry == "")
        return nullptr;

    std::string l = this->entry->getEntryFeatureAt(label, feature);

    if(this->type == (int) DataSet::Categorical){
        if(entry <l && this->left != nullptr){
            if(! this->left->visited && ! this->left->marked)
                return this->left->traverse(query);
        }
        else if (entry >= l && this->right != nullptr){
            if(! this->right->visited && ! this->right->marked)
                return this->right->traverse(query);
        }
    }
    else{
        if(stod(entry) < stod(l) && this->left != nullptr){
            if(! this->left->visited && ! this->left->marked)
                return this->left->traverse(query);
        }
        else if(stod(entry) > stod(l) && this->right != nullptr){
            if(! this->right->visited && ! this->right->marked)
                return this->right->traverse(query);
        }
    }

    if(! this->marked && ! this->visited){
        this->visited = true;
        return this;
    }
    return nullptr;
}

void KDTree::unvisitChildren(){
    this->visited = false;

    if(left != nullptr){
        left->unvisitChildren();
    }
    if(right != nullptr){
        right->unvisitChildren();
    }
}

void KDTree::unmarkChildren(){
    this->marked = false;

    if(left != nullptr){
        left->unmarkChildren();
    }
    if(right != nullptr){
        right->unmarkChildren();
    }
}

KDTree* KDTree::query(std::map<std::string, std::string> query){
    double best = -1;


    // Descend
    KDTree* node = this->traverse(query);
    KDTree* curr = nullptr;
    Metric* metric = new Metric();

    while(node != nullptr){
        double distance;
        double boundary;

        if(node->type == (int) DataSet::Categorical){
            boundary = std::min(metric->levenshtein(node->bounds.first , query[node->feature]),
                                metric->levenshtein(node->bounds.second , query[node->feature]));
        }
        else{
            boundary = std::min(metric->euclidean(node->bounds.first , query[node->feature]),
                                metric->euclidean(node->bounds.second , query[node->feature]));
        }

        distance = sqrt(node->getDistance(query));

        if(best == -1 || distance < best){
            best = distance;
            curr = node;
        }

        // Hyperplane testing. Check if there are possibly other neighbors.
        if(boundary < best && boundary >= 0){
            node = node->traverse(query);
        }

        else{
            node = node->parent;
        }
    }
    // Mark the current node
    if(curr != nullptr)
        curr->marked = true;
    return curr;
}

int KDTree::getLabel(){
    return this->label;
}

DataSet* KDTree::getEntries(){
    return this->entry;
}

double KDTree::getDistance(std::map<std::string, std::string> query){
    double dist = 0;
    Metric* m = new Metric();
    if(this->entry == nullptr)
        return 0;

    for(int i = 0; i < entry->getFeatureCount(); i++){
        if(entry->getFeatureAt(i).first == target)
            continue;

        if(entry->getFeatureAt(i).second == DataSet::Categorical)
            dist += m->levenshtein(query[entry->getFeatureAt(i).first], entry->getEntryFeatureAt(label, i));
        else{
            double temp = m->squareEuclidean(query[entry->getFeatureAt(i).first], entry->getEntryFeatureAt(label, i));
            dist += temp * temp;
        }
    }

    return dist;
}
