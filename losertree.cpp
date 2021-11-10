#include<iostream>
#include<cmath>
#include<cstring>
#include<sstream>
#include<fstream>
using namespace std;
namespace shuru
{
    template < typename T > std::string to_string( const T& n )  //to_string()  
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}
template<typename T>
struct playnode
{
	int id;
	T key;
	bool operator <= (const playnode &x) const//id升序排序，id一样的话key升序排序 
	{
		if(id!=x.id)
		{
			if(id<=x.id)
			    return true;
			else
			    return false;    
		}
		else
		{
			if(key<=x.key)
			    return true;
			else
			    return false;     
		}
	}
};
template<class T>
class LoserTree
{
	public: 
		LoserTree(T *theplayer,int then)//初始化 
		{
			Build(theplayer,then);
		}
		void Build(T *theplayer,int then);//建立 
		void play(int p,int l,int r);//插入结点 
		void replay(int x);//更新输者树 
		int loser()//返回输者 
		{
			if(n==0) 
				return 0;
			return tree[1];
		}
	private:
		int Buffer;//缓冲区 
		int n;//要排序的数的数量 
		int lowExt;//表示最后一层有多少点 
		int offset;//表示一个偏移量 
		int *tree;//存储最优的点的序列中的编号 
		T* player;//待处理的顺串/序列的当前的玩家 
};
template<class T>
void LoserTree<T>::Build(T * theplayer,int then)
{
	player=theplayer;//待处理的顺串/序列的当前的玩家
	n=then;//输入的元素数量 
	tree=new int [n+1];//建立树节点 
	int s=log(n-1)/log(2);//层数就是log以2为底n-1的值 
	s=pow(2,s);//满二叉树的节点数 
	lowExt=2*(n-s);//剩余的就是最后一层的结点了 
	offset=2*s-1;//偏移量 
	int i=0; 
	for(int i=2;i<=lowExt;i=i+2)//处理最后一层
		play(i+offset>>1,i-1,i);//与上一个相比较 
    if(n%2==1)//处理单个的情况，让它和左边兄弟的合并 
	{
		play(n/2,tree[n-1],lowExt+1); 
		i=lowExt+3;
	} 
	else 
		i=lowExt+2;
	for(;i<=n;i=i+2)// 处理非最后一层 
		play((i-lowExt+n-1)/2,i-1,i); 
}
template<class T>
void LoserTree<T>::play(int p,int l,int r)//p是当前树节点（放输者），l是待插入的左边的玩家，r为待插入的右边的玩家 
{
	if(player[l]<=player[r])//插入的是输者（大的那个），记录下输者为边然后下一层比较 
	    tree[p]=l;
	else 
	    tree[p]=r;
	//p为偶数，只竞争一次，且只发生在最后一层，此时所有玩家都在树中，直到顶层    
	while((p%2==1)&&p>1)//右边的玩家刚好是偶数 
	{
		if(player[tree[p-1]]<=player[tree[p]])//同上 
		{
			tree[p/2]=tree[p-1];
		}
		else 
		    tree[p/2]=tree[p];
		p=p/2; 
	}
}
template<class T>
void LoserTree<T>::replay(int k)//更新输者树，k为玩家的值 
{
	if(k<0||k>n) 
	return ;
	int now,l,r;//玩家的值，左玩家，右玩家 
	if(k<=lowExt)//玩家在底层 
	{
		now=offset+k>>1;  //father节点的编号 
		l=2*now-offset; 
		r=l+1;
	}
	else//玩家在倒数第二 
	{
		now=(k-lowExt+n-1)/2;
		if(2*now==n-1)//特殊的分数 
		{
			l=tree[2*now];
			r=k;
		} 
		else 
		{
			l=2*now-(n-lowExt-1);
			r=l+1;
		}
	}
	if(player[l]<=player[r])//同上 
	    tree[now]=l;
	else 
	    tree[now]=r;
	if(now==n-1&&(n%2==1))
	{
		now>>=1;
		if(player[tree[n-1]]<=player[lowExt+1])
			tree[now/2]=tree[n-1];
		else 
		    tree[now/2]=lowExt+1;
	}
	now>>=1;
	for(;now>=1;now>>=1)
	{
		if(player[tree[now*2]]<=player[tree[now*2+1]])
		 	tree[now]=tree[now*2];
		else 
		    tree[now]=tree[now*2+1];
	}
}
template<class T>
class ExternalSort
{
	public: 
		void Kway();  //合并 
		void Solve();
		void replay();
		int count;
	private:
		playnode<T> *player;
		int Buffer;
		int n;
		int maxid;
		int *cnt;
};
template<class T>
void ExternalSort<T>::Solve()
{
	cin>>n;//输入的字符数量 
	Buffer=10;//缓冲区数量 
	count=0;//记录操作次数 
	cnt = new int [n+1];//记录名次 
	playnode<int> *player=new playnode<T>[Buffer+1];
	for(int i=1;i<=Buffer;i++)
	{
		cin>>player[i].key;//输入每个字符 
		player[i].id=1;//初始化 
		count++;//操作次数++ 
	}
	LoserTree<playnode<T>> tree(player,Buffer);//建立树 
	maxid=0;//最大的结点初始化 
	int now=Buffer+1;
	for(int i=1;i<=n;i++) 
	    cnt[i]=0;//初始化 
	for(int i=Buffer+1;i<=n;i++)//在缓冲区之外插入 
	{
		int loser = tree.loser();//返回输着 
		int id=player[loser].id;//确定编号 
		cnt[id]++;//位次加一 
		maxid=max(maxid,id); 
		string file=shuru::to_string(id)+".in";
		ofstream fout(file.c_str(), ios::app);
		if(cnt[id]>1)//第一个输出就输出数字，其余每次输出就输出空格+数字，这样就达到了最后一个数字没有空格的要求了 
		    fout<<' ';
		fout<<player[loser].key;
		count++;//操作个数+1 
		fout.close();
		int key;
		cin>>key;//在缓冲区之外的玩家要插入了 
		    count++;//操作个数+1 
		if(key<player[loser].key)//取编号的最大值并赋值 
		    player[loser].id++;
		player[loser].key=key; 
		tree.replay(loser);
	}
	for(int i=1;i<=Buffer;i++)//缓冲区内部也是一样的 
	{
		int loser = tree.loser();
		int id=player[loser].id;
		cnt[id]++;
		maxid=max(maxid,id);
		string file=shuru::to_string(id)+".in";
		ofstream fout(file.c_str(), ios::app);
		if(cnt[id]>1) 
		    fout<<' ';
		fout<<player[loser].key;
		fout.close();
		player[loser].key=0;//下两行为初始化 
		player[loser].id=n+1;
		tree.replay(loser);
	}
}
template<class T>
void ExternalSort<T>::Kway()
{
	int div=Buffer/maxid,mod=Buffer%maxid;//定义商和余数 
	player = new playnode<int>[Buffer+1];
	ifstream  *file=new ifstream[maxid+1];
	for(int i=1;i<=maxid;i++) 
	{
		string tmp=shuru::to_string(i)+".in";
		const char *name=tmp.c_str();
		file[i].open(tmp.c_str());
	}
	int x;
	for(int i=1;i<=maxid;i++)//调整名次 
	{	
		if(i<=mod) 
		    x=1;
		else 
		    x=0;
		cnt[i]=min(cnt[i],div+x);
	}
	for(int i=1;i<=Buffer;i++)//初始化 
	    player[i].id=2;
	int tot=0;//名次总数 
	int *belong = new int [n+1];//用数组表示父亲 
	for(int i=1;i<=maxid;i++)
	{
		for(int j=1;j<=cnt[i];j++)
		{
			file[i]>>player[tot+j].key;
			player[tot+j].id=1;
			belong[tot+j]=i;
		}
		tot=tot+cnt[i];
	}
	LoserTree<playnode<T> > tree(player,Buffer);
	for(int i=1;i<=n;i++)//同上 
	{
		int loser = tree.loser();
		int id=belong[loser];
		string out="out.txt";
		ofstream fout(out.c_str(), ios::app);
		fout<<player[loser].key<<" ";
		cnt[id]++;
		count++;
		if(file[id].peek()!=EOF)//没输入完就输入，否则就确定编号，peek是为了正常返回 
		    file[id]>>player[loser].key;
		else 
		    player[loser].id=2;
		tree.replay(loser);
	}
	return ;
}
int main()
{ 
	freopen("in.txt","r",stdin);//输入文件包括一个序列长度n，和一个序列。
	ExternalSort<int> A;
	A.Solve();
	A.Kway();
	cout<<"此次执行的操作为（次）:"<<A.count;
	return 0; 
}

