/* 请实现两个函数，分别用来序列化和反序列化二叉树 */

/*
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int x) :
            val(x), left(NULL), right(NULL) {
    }
};
*/
class Solution {
public:
    char* Serialize(TreeNode *root) {    
        
        string str;
        
        // 如果为空,在str插入字符“#”代表空，直接返回
        if(nullptr == root){
          /*str.append("#,");
            return str.to_string();
          */
            return "#";
        }
        
        // 将前序遍历的元素添加到字符串末尾
        str.append(to_string(root->val)+",");
        
        str.append(Serialize(root->left));
        str.append(Serialize(root->right));
        
        // 由于返回值是char *，所以创建一个char数组来返回最终字符串
        char *pstr = new char[str.size()];
        strcpy(pstr, str.c_str());
        /*
        char *left_str = Serialize(root->left);
        char *right_str = Serialize(root->right);
        
        char *pstr = new char[strlen(left_str)+ strlen(right_str) + str.size()];
        
        // 递归式的每次将子树按前序顺序，copy给目标串
        strcpy(pstr, str.c_str());
        strcat(pstr, left_str);
        strcat(pstr, right_str);
        */
        
        return pstr;
    }
    
    TreeNode* outPutDes(char *&str){
                
        // 如果为空指针，直接返回
        if(nullptr == str )
            return NULL;
        
        /*
            如果遍历到空值，表示该结点没有子结点
            返回，去遍历下一个
        */
        if('#' == (*str)){
            str++;
            return NULL;
        }
        
        int num = 0;
        while(',' != (*str)){
            num = num * 10 + (*(str++) - '0');
           
        }
         str++;
        TreeNode *root = new TreeNode(num);
        root->left = outPutDes(str);
        root->right = outPutDes(str);
        
        return root;
    }
    
    TreeNode* Deserialize(char *str) {
        TreeNode* ret = outPutDes(str);
        return ret;
    }
};
